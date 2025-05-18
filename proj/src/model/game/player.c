#include "player.h"
#include "lcom/lcf.h"
#include "../Sprite.h"
#include "../../view/view.h"
#include <stdlib.h>
#include <math.h>

extern uint8_t scan_code;
extern uint8_t *sec_frame_buffer;
extern vbe_mode_info_t vg_mode_info;

uint8_t *maze_buffer = NULL;

double fov_angle = 60.0; // Valor inicial de 60 graus

/* static */ void playerIsAtMaxSpeed(Player *player){
    if(player->sprite->xspeed >= player->max_speed){
        player->sprite->xspeed = player->max_speed;
    }
    if(player->sprite->yspeed >= player->max_speed){
        player->sprite->yspeed = player->max_speed;
    }
}

/* static */ void playerStopped(Player *player){
    if(player->moved == 0){
        player->sprite->xspeed = PLAYER_DEFAULT_SPEED;
        player->sprite->yspeed = PLAYER_DEFAULT_SPEED;
    }
}

// Create a new player
Player *create_player(Sprite *sprite) {
    
    Player *player = (Player *)malloc(sizeof(Player));
    if (!player) return NULL;
    player->health = PLAYER_HEALTH;
    player->sprite = sprite;
    player->moved = 0;
    player->max_speed = PLAYER_MAX_SPEED; // Set default max speed
    player->acceleration = PLAYER_ACCELERATION; // Set default acceleration
    return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
    if (!player) return;

    if (player->sprite) {
        destroy_sprite(player->sprite);
    }
    free(player);
}

int draw_player(Player *player) {
    draw_sprite_pos_to_delta(player->sprite, delta, sec_frame_buffer);
    return 0;
}

void init_maze_buffer() {
  if (maze_buffer == NULL) {
    uint32_t bytesPerPixel = (vg_mode_info.BitsPerPixel + 7) / 8;
    uint32_t frameSize = vg_mode_info.XResolution * vg_mode_info.YResolution * bytesPerPixel;
    maze_buffer = (uint8_t*) malloc(frameSize);
    if (maze_buffer == NULL) {
      printf("Erro ao alocar maze_buffer\n");
    }
  }
}

void free_maze_buffer() {
    if (maze_buffer != NULL) {
        free(maze_buffer);
        maze_buffer = NULL;
    }
}


void keyboard_handler(Player *player, Maze* maze) {
    double x_changer = 0;
    double y_changer = 0;

    if (player == NULL) return;

    bool moved = 0;

    switch (scan_code) {
        case KEY_W:
            x_changer = cos(delta);
            y_changer = sin(delta);
            direction = delta;
            moved = 1;
            break;

        case KEY_A:
          x_changer = sin(delta);
          y_changer = -cos(delta);
          direction = delta - M_PI / 2;
          moved = 1;
          break;

        case KEY_S:
          x_changer = -cos(delta);
          y_changer = -sin(delta);
          direction = delta + M_PI;
          moved = 1;
          break;

        case KEY_D:
          x_changer = -sin(delta);
          y_changer = cos(delta);
          direction = delta + M_PI / 2;
          moved = 1;
          break;

        case KEY_X:
          player->sprite->x = x_mouse;
          player->sprite->y = y_mouse;
          moved = 0;
          break;

        case KEY_SPACE:
         fov_angle += 10;
         break;

        case KEY_C:
         fov_angle -= 10;
         break;

        default:
          break;
    }
    if (fov_angle < 10) fov_angle = 10;
    if (fov_angle > 180) fov_angle = 180;

    if (moved) {
        player->sprite->xspeed += player->acceleration;
        player->sprite->yspeed += player->acceleration;
        playerIsAtMaxSpeed(player);

        double new_x = player->sprite->x + x_changer * player->sprite->xspeed;
        double new_y = player->sprite->y + y_changer * player->sprite->yspeed;

        // Verifica colisão antes de atualizar a posição
        if (!check_collision(maze, new_x, new_y, 
                           player->sprite->width, player->sprite->height)) {
            player->sprite->x = new_x;
            player->sprite->y = new_y;
        }
    }

    player->moved = moved;
    playerStopped(player);
}

void game_update_delta(Player *player)
{
    double player_center_x = player->sprite->x + player->sprite->width/2.0;
    double player_center_y = player->sprite->y + player->sprite->height/2.0;
    
    double dx = x_mouse - player_center_x;
    double dy = y_mouse - player_center_y;
    
    delta = atan2(dy, dx);
}

void mouse_handler(Player *player, struct packet pp) {
    x_mouse += pp.delta_x * 0.5;
    y_mouse -= pp.delta_y * 0.5;
    
    uint16_t screen_width = vg_mode_info.XResolution;
    uint16_t screen_height = vg_mode_info.YResolution;
    
    const int CURSOR_WIDTH = 16;
    const int CURSOR_HEIGHT = 16;
    
    if (x_mouse < 0) x_mouse = 0;
    if (y_mouse < 0) y_mouse = 0;
    if (x_mouse > screen_width - CURSOR_WIDTH) 
        x_mouse = screen_width - CURSOR_WIDTH;
    if (y_mouse > screen_height - CURSOR_HEIGHT) 
        y_mouse = screen_height - CURSOR_HEIGHT;
    
    game_update_delta(player);
}

void game_draw_cursor() {
    draw_xpm_at_pos((xpm_map_t) cross, (int) x_mouse, (int) y_mouse, sec_frame_buffer);
}

void game_draw_fov_cone(Player *player, Maze* maze) {
    if (!player || !maze) return;

    double x = player->sprite->x + player->sprite->width / 2.0;
    double y = player->sprite->y + player->sprite->height / 2.0;

    double fov_radius = FOV_RADIUS;
    double cone_half_angle = fov_angle * M_PI / 360.0; // Metade do ângulo do cone em radianos

    // Precompute direção do vetor unitário
    double dir_x = cos(delta);
    double dir_y = sin(delta);

    // Precompute cosseno para o produto escalar
    double cos_half_angle = cos(cone_half_angle);
    double cos_half_angle_sq = cos_half_angle * cos_half_angle;

    // Apenas verifica pixels num quadrado ao redor do jogador (otimização massiva)
    int x_start = (int)(x - fov_radius);
    int x_end = (int)(x + fov_radius);
    int y_start = (int)(y - fov_radius);
    int y_end = (int)(y + fov_radius);

    // Limita valores às bordas da tela
    x_start = (x_start < 0) ? 0 : x_start;
    x_end = (x_end >= vg_mode_info.XResolution) ? vg_mode_info.XResolution - 1 : x_end;
    y_start = (y_start < 0) ? 0 : y_start;
    y_end = (y_end >= vg_mode_info.YResolution) ? vg_mode_info.YResolution - 1 : y_end;

    uint32_t bytesPerPixel = (vg_mode_info.BitsPerPixel + 7) / 8;
    uint32_t frameSize = vg_mode_info.XResolution * vg_mode_info.YResolution * bytesPerPixel;

    for (int y_pixel = y_start; y_pixel <= y_end; y_pixel++) {
        for (int x_pixel = x_start; x_pixel <= x_end; x_pixel++) {
            if (x_pixel == (int)x && y_pixel == (int)y) continue;

            double dx = x_pixel - x;
            double dy = y_pixel - y;

            // Apenas multiplicações em vez de sqrt para melhor desempenho
            double dist_sq = dx * dx + dy * dy;
            if (dist_sq > fov_radius * fov_radius) continue;

            double dot_product = dx * dir_x + dy * dir_y;
            if (dot_product < 0) continue; // Atrás do jogador

            // Verifica se está dentro do cone usando comparação de quadrados
            if ((dot_product * dot_product) >= (dist_sq * cos_half_angle_sq)) {
                uint32_t index = (vg_mode_info.XResolution * y_pixel + x_pixel) * bytesPerPixel;
                
                if (index < frameSize) {
                    memcpy(&sec_frame_buffer[index], &maze_buffer[index], bytesPerPixel);
                }
            }
        }
    }
}
