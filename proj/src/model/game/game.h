#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "player.h"

typedef enum {
    MENU,
    LEVEL,
    VICTORY,
    GAME_OVER,
    EXIT
} State;

typedef struct  {
    State state;
    uint8_t level_number;
    Level level;
} Game;

void main_game_loop();

#endif
