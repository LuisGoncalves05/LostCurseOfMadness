#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "game_over.h"
#include <math.h>

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
