#include "game.h"
#include "player.h"
#include "maze.h"
#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/timer/i8254.h"
#include "drivers/video/gpu.h"
#include "model/game/level.h"
#include "model/sprite.h"
#include <math.h>

struct Game {
  State state;
  uint8_t level_number;
  uint8_t score;
  Level *level;
};

double delta;
double direction;
double x_mouse;
double y_mouse;
double fov_angle = 60.0;

static void menu_init(Game *game) {
  printf("in menu\n");
}

static void level_init(Game *game) {
  game->level = create_level(game->level_number);
}

static void victory_init(Game *game) {
  game->score = game->level_number;
  game->level_number++;
}

static void game_over_init(Game *game) {
  game->score = game->level_number;
  game->level_number = 0;
}

static void exit_init(Game *game) {
  printf("exiting game\n");
}

static void (*game_init[])(Game *game) = {
  menu_init,
  level_init,
  victory_init,
  game_over_init,
  exit_init};

static void state_init(Game *game) {
  game_init[game->state](game);
}

static void menu_destroy(Game *game) {
  printf("exiting menu\n");
}

static void level_destroy(Game *game) {
  printf("exiting level\n");
  destroy_level(game->level);
}

static void victory_destroy(Game *game) {
  printf("exiting victory state\n");
}

static void game_over_destroy(Game *game) {
  printf("exiting game over state\n");
}

static void exit_destroy(Game *game) {
  printf("This function should never be called\n");
}

static void (*game_destroy[])(Game *game) = {
  menu_destroy,
  level_destroy,
  victory_destroy,
  game_over_destroy,
  exit_destroy};

static void state_destroy(Game *game) {
  game_destroy[game->state](game);
}

static void set_state(Game *game, State new_state) {
  // printf("Changing state from %d to %d\n", game->state, new_state);
  state_destroy(game);
  game->state = new_state;
  state_init(game);
}

static void game_update_delta(Game *game) {
  Player *player = get_player(game->level);
  Sprite *player_sprite = get_sprite(player);
  double player_center_x = player_sprite->x + player_sprite->width / 2.0;
  double player_center_y = player_sprite->y + player_sprite->height / 2.0;

  double dx = x_mouse - player_center_x;
  double dy = y_mouse - player_center_y;

  delta = atan2(dy, dx);
}

static void game_draw_cursor() {
  Sprite *mouse = create_sprite(cross, x_mouse, y_mouse, 0, 0);
  draw_sprite(mouse, sec_frame_buffer);
}

static void game_draw_fov_cone(Game *game) {
  Player *player = get_player(game->level);
  Sprite *player_sprite = get_sprite(player);
  Maze *maze = get_maze(game->level);
  if (!player || !maze)
    return;

  double x = player_sprite->x + player_sprite->width / 2.0;
  double y = player_sprite->y + player_sprite->height / 2.0;

  double fov_radius = FOV_RADIUS;
  double cone_half_angle = fov_angle * M_PI / 360.0; // Metade do ângulo do cone em radianos

  // Precompute direção do vetor unitário
  double dir_x = cos(delta);
  double dir_y = sin(delta);

  // Precompute cosseno para o produto escalar
  double cos_half_angle = cos(cone_half_angle);
  double cos_half_angle_sq = cos_half_angle * cos_half_angle;

  // Apenas verifica pixels num quadrado ao redor do jogador (otimização massiva)
  int x_start = (int) (x - fov_radius);
  int x_end = (int) (x + fov_radius);
  int y_start = (int) (y - fov_radius);
  int y_end = (int) (y + fov_radius);

  // Limita valores às bordas da tela
  x_start = (x_start < 0) ? 0 : x_start;
  x_end = (x_end >= x_res) ? x_res - 1 : x_end;
  y_start = (y_start < 0) ? 0 : y_start;
  y_end = (y_end >= y_res) ? y_res - 1 : y_end;


  for (int y_pixel = y_start; y_pixel <= y_end; y_pixel++) {
    for (int x_pixel = x_start; x_pixel <= x_end; x_pixel++) {
      if (x_pixel == (int) x && y_pixel == (int) y)
        continue;

      double dx = x_pixel - x;
      double dy = y_pixel - y;

      // Apenas multiplicações em vez de sqrt para melhor desempenho
      double dist_sq = dx * dx + dy * dy;
      if (dist_sq > fov_radius * fov_radius)
        continue;

      double dot_product = dx * dir_x + dy * dir_y;
      if (dot_product < 0)
        continue; // Atrás do jogador

      // Verifica se está dentro do cone usando comparação de quadrados
      if ((dot_product * dot_product) >= (dist_sq * cos_half_angle_sq)) {
        uint32_t index = (x_res * y_pixel + x_pixel) * bytes_per_pixel;

        if (index < frame_size) {
          memcpy(&sec_frame_buffer[index], &maze_buffer[index], bytes_per_pixel);
        }
      }
    }
  }
}

static int game_draw_player(Game *game) {
  Player *player = get_player(game->level);
  draw_sprite_rotated(get_sprite(player), delta, sec_frame_buffer);
  return 0;
}

static void menu_timer_handler(Game *game) {
}

static void level_timer_handler(Game *game) {
  Level *current_level = game->level;
  if (!current_level)
    return;

  Player *player = get_player(game->level);
  Maze *maze = get_maze(current_level);
  if (!player || !maze)
    return;

  clear(sec_frame_buffer);
  clear(maze_buffer);

  draw_maze(maze, maze_buffer);
  game_draw_fov_cone(game);
  game_draw_player(game);
  game_draw_cursor();

  vga_flip_pages();
}

static void victory_timer_handler(Game *game) {
  printf("Win!\n");
}

static void game_over_timer_handler(Game *game) {
  printf("GAME OVER\n");
}

static void exit_timer_handler(Game *game) {
  printf("exiting\n");
}

static void (*game_timer_handlers[])(Game *game) = {
  menu_timer_handler,
  level_timer_handler,
  victory_timer_handler,
  game_over_timer_handler,
  exit_timer_handler
};

void game_timer_handler(Game *game) {
  game_timer_handlers[game->state](game);
}

static void menu_keyboard_handler(Game *game) {
  if (scan_code == ESC_BREAK_CODE)
    set_state(game, EXIT);
}

static void level_keyboard_handler(Game *game) {
  Player *player = get_player(game->level);
  Sprite *player_sprite = get_sprite(player);
  Maze *maze = get_maze(game->level);

  double x_changer = 0;
  double y_changer = 0;

  if (player == NULL)
    return;

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
      player_sprite->x = x_mouse;
      player_sprite->y = y_mouse;
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
  if (fov_angle < 30)
    fov_angle = 30;
  if (fov_angle > 120)
    fov_angle = 120;

  if (moved) {
    int acceleration = get_acceleration(player);
    player_sprite->xspeed += acceleration;
    player_sprite->yspeed += acceleration;
    playerIsAtMaxSpeed(player);

    double new_x = player_sprite->x + x_changer * player_sprite->xspeed;
    double new_y = player_sprite->y + y_changer * player_sprite->yspeed;

    // Verifica colisão antes de atualizar a posição
    if (!check_collision(maze, new_x, new_y,
                         player_sprite->width, player_sprite->height)) {
      player_sprite->x = new_x;
      player_sprite->y = new_y;
    }
  }

  set_moved(player, moved);
  playerStopped(player);

  if (scan_code == ESC_BREAK_CODE)
    set_state(game, EXIT);
}

static void victory_keyboard_handler(Game *game) {
  printf("victory_keyboard_handler: To be implemented\n");
}

static void game_over_keyboard_handler(Game *game) {
  printf("game_over_keyboard_hanlder: To be implemented\n");
}

static void exit_keyboard_handler(Game *game) {
  printf("This function should never be called\n");
}

static void (*game_keyboard_handlers[])(Game *game) = {
  menu_keyboard_handler,
  level_keyboard_handler,
  victory_keyboard_handler,
  game_over_keyboard_handler,
  exit_keyboard_handler
};

void game_keyboard_handler(Game *game) {
  game_keyboard_handlers[game->state](game);
}

static void menu_mouse_handler(Game *game) {
  printf("menu_mouse_handler: To be implemented\n");
}

static void level_mouse_handler(Game *game) {
  x_mouse += pp.delta_x * 0.5;
  y_mouse -= pp.delta_y * 0.5;

  const int CURSOR_WIDTH = 16;
  const int CURSOR_HEIGHT = 16;

  if (x_mouse < 0)
    x_mouse = 0;
  if (y_mouse < 0)
    y_mouse = 0;
  if (x_mouse > y_res - CURSOR_WIDTH)
    x_mouse = y_res - CURSOR_WIDTH;
  if (y_mouse > x_res - CURSOR_HEIGHT)
    y_mouse = x_res - CURSOR_HEIGHT;

  game_update_delta(game);
}

static void victory_mouse_handler(Game *game) {
  printf("victory_mouse_handler: To be implemented\n");
}

static void game_over_mouse_handler(Game *game) {
  printf("game_over_mouse_handler: To be implemented\n");
}

static void exit_mouse_handler(Game *game) {
  printf("This function should never be called\n");
}

static void (*game_mouse_handlers[])(Game *game) = {
  menu_mouse_handler,
  level_mouse_handler,
  victory_mouse_handler,
  game_over_mouse_handler,
  exit_mouse_handler};

void game_mouse_handler(Game *game) {
  game_mouse_handlers[game->state](game);
}


/* public functions */

Game *create_game() {
  Game *game = (Game *) malloc(sizeof(Game));
  if (!game)
    return NULL;

  game->level_number = 0;
  game->score = 0;
  game->state = LEVEL;
  state_init(game);

  return game;
}

void destroy_game(Game *game) {
  if (game != NULL) {
    if (game->state == LEVEL)
      destroy_level(game->level);
    free(game);
  }
}

State get_state(Game *game) {
  return game->state;
}
