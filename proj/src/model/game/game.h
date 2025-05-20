#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "model/menu/game_over.h"
#include "model/menu/main_menu.h"
#include <math.h>

#include "model/keys.h"
#include "controller/video/gpu.h"
#include "controller/keyboard/i8042.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/timer/i8254.h"
#include "view/view.h"
#include "model/Sprite.h"
#include "model/game/level.h"

extern uint8_t *maze_buffer;

double	delta;
double	direction;

double	x_mouse;
double	y_mouse;

typedef enum {
    MENU,
    LEVEL,
    VICTORY,
    GAME_OVER,
    EXIT
} State;

typedef struct Game Game;

Game *create_game();
void destroy_game(Game *game);

State get_state(Game *game);

void game_timer_handler(Game* game);
void game_keyboard_handler(Game* game);
void game_mouse_handler(Game* game);

#endif
