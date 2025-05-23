#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>

#include "model/level/level.h"
#include "model/menu/game_over.h"
#include "model/menu/main_menu.h"
#include "model/utils/cursor.h"
#include "model/utils/keys.h"
#include "model/utils/sprite.h"

#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/timer/i8254.h"
#include "drivers/video/gpu.h"

#include "assets/xpm/cursor.xpm"

#define FOV_RADIUS 600

extern uint8_t *maze_buffer;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res;
extern uint16_t y_res;
extern uint32_t frame_size;

extern uint8_t scan_code;
extern struct packet pp;

typedef struct Game Game;

typedef enum {
  MENU,
  LEVEL,
  VICTORY,
  GAME_OVER,
  EXIT
} State;

Game *create_game();
void destroy_game(Game *game);

State get_state(Game *game);

void game_timer_handler(Game *game);
void game_keyboard_handler(Game *game);
void game_mouse_handler(Game *game);

#endif
