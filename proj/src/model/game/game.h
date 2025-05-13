#ifndef GAME_H
#define GAME_H

#include "level.h"

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
void main_game_loop();

State get_state(Game *game);

void game_timer_handler(Game* game);
int handle_keyboards();

#endif
