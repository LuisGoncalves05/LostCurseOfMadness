#include "game.h"
#include "level/bullet.h"

struct Game {
  State state;
  uint8_t level_number;
  uint8_t score;
  Level *level;
  Cursor *cursor;
  union {
    struct GameOver *game_over;
    struct MainMenu *main_menu;
  } menu;
  bool shooting;
};

static void menu_init(Game *game) {
  game->menu.main_menu = create_main_menu();
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

  game->menu.game_over = create_game_over();
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
  destroy_main_menu(game->menu.main_menu);
}

static void level_destroy(Game *game) {
  destroy_level(game->level);
}

static void victory_destroy(Game *game) {
}

static void game_over_destroy(Game *game) {
  destroy_game_over(game->menu.game_over);
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
  state_destroy(game);
  game->state = new_state;
  state_init(game);
}

static void menu_timer_handler(Game *game) {
  clear(sec_frame_buffer);

  draw_main_menu(game->menu.main_menu, sec_frame_buffer);
  draw_cursor(game->cursor, sec_frame_buffer);

  vga_flip_pages();
}

static void level_timer_handler(Game *game) {
  clear(sec_frame_buffer);

  draw_level(game->level, pp);
  draw_cursor(game->cursor, sec_frame_buffer);
  
  vga_flip_pages();

  if (get_player_state(get_player(game->level)) == PLAYER_DYING) set_state(game, GAME_OVER);
}

static void victory_timer_handler(Game *game) {
  printf("Win!\n");
}

static void game_over_timer_handler(Game *game) {
  clear(sec_frame_buffer);

  draw_game_over(game->menu.game_over, sec_frame_buffer);
  draw_cursor(game->cursor, sec_frame_buffer);

  vga_flip_pages();
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
  if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D)
    main_menu_change_button(game->menu.main_menu);
  if (scan_code == KEY_ENTER) {
    ButtonType button = main_menu_get_button(game->menu.main_menu);
    if (button == BUTTON_PLAY) {
      set_state(game, LEVEL);
    }
    else if (button == BUTTON_EXIT) {
      set_state(game, EXIT);
    }
  }
}

static void level_keyboard_handler(Game *game) {
  level_update_position(game->level, scan_code);
  if (scan_code == ESC_BREAK_CODE) set_state(game, MENU);
}

static void game_over_keyboard_handler(Game *game) {
  if (scan_code == ESC_BREAK_CODE)
    set_state(game, EXIT);
  if (scan_code == KEY_W || scan_code == KEY_S || scan_code == KEY_A || scan_code == KEY_D)
    game_over_change_button(game->menu.game_over);
  if (scan_code == KEY_ENTER) {
    ButtonType button = game_over_get_button(game->menu.game_over);
    if (button == BUTTON_MENU) {
      set_state(game, MENU);
    }
    else if (button == BUTTON_EXIT) {
      set_state(game, EXIT);
    }
  }
}

static void victory_keyboard_handler(Game *game) {
  printf("victory_keyboard_handler: To be implemented\n");
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
  ButtonType button = BUTTON_NONE;
  if (pp.lb)
    button = main_menu_click_handler(game->menu.main_menu, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
  if (button == BUTTON_PLAY) {
    set_state(game, LEVEL);
  }
  else if (button == BUTTON_EXIT) {
    set_state(game, EXIT);
  }
}

static void level_mouse_handler(Game *game) {
  update_delta(game->level, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
  if (pp.lb) {
    if (!game->shooting) level_shoot(game->level);
    game->shooting = true;
  } else {
    game->shooting = false;
  }
}

static void victory_mouse_handler(Game *game) {
  printf("victory_mouse_handler: To be implemented\n");
}

static void game_over_mouse_handler(Game *game) {
  ButtonType button = BUTTON_NONE;
  if (pp.lb)
    button = game_over_click_handler(game->menu.game_over, cursor_get_x(game->cursor), cursor_get_y(game->cursor));
  if (button == BUTTON_MENU) {
    set_state(game, MENU);
  }
  else if (button == BUTTON_EXIT) {
    set_state(game, EXIT);
  }
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
  cursor_update(game->cursor, pp.delta_x * 0.5, -pp.delta_y * 0.5);
  game_mouse_handlers[game->state](game);
}

/* public functions */

Game *create_game() {
  Game *game = (Game *) malloc(sizeof(Game));
  if (!game)
    return NULL;

  game->level_number = 0;
  game->score = 0;
  game->state = MENU;
  game->cursor = create_cursor((xpm_map_t) cursor);
  game->shooting = false;
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
