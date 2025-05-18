#include "player.h"
#include "lcom/lcf.h"
#include "../Sprite.h"
#include "../../view/view.h"
#include <stdlib.h>

extern uint8_t scan_code;
extern uint8_t *sec_frame_buffer;

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

        default:
          break;
    }

    if (moved) {
        player->sprite->xspeed += player->acceleration;
        player->sprite->yspeed += player->acceleration;
        playerIsAtMaxSpeed(player);

        // Calcula a nova posição
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

void mouse_handler(Player *player, struct packet pp){
	x_mouse += pp.delta_x * 0.5;
	y_mouse -= pp.delta_y * 0.5;
	game_update_delta(player);
}

void game_draw_cursor() {
    draw_xpm_at_pos((xpm_map_t) cross, (int) x_mouse, (int) y_mouse, sec_frame_buffer);
}
