#include "game.h"

#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/timer/i8254.h"
#include "controller/video/gpu.h"
#include "model/sprite.h"
#include "model/game/level.h"
#include "view/view.h"

extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;

#include "controller/keyboard/i8042.h"
extern uint8_t scan_code;
extern struct packet pp;

struct Game {
  State state;
  uint8_t level_number;
  uint8_t score;
  struct Level *level;
};

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
  game_draw_fov_cone(player, maze);
  draw_player(player);
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
  exit_timer_handler};

void game_timer_handler(Game *game) {
  game_timer_handlers[game->state](game);
}

static void menu_keyboard_handler(Game *game) {
  if (scan_code == ESC_BREAK_CODE)
    set_state(game, EXIT);
}

static void level_keyboard_handler(Game *game) {
  keyboard_handler(get_player(game->level), get_maze(game->level));
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
  exit_keyboard_handler};

void game_keyboard_handler(Game *game) {
  game_keyboard_handlers[game->state](game);
}

static void menu_mouse_handler(Game *game) {
  printf("menu_mouse_handler: To be implemented\n");
}

static void level_mouse_handler(Game *game) {
  mouse_handler(get_player(game->level), pp);
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
    if (game->state == LEVEL) destroy_level(game->level);
        free(game);
  }
}

State get_state(Game *game) {
  return game->state;
}
