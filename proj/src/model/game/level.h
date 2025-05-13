#ifndef LEVEL_H
#define LEVEL_H

#include "maze.h"
#include "player.h"

typedef struct Level Level;

Level *create_level(uint8_t number);
void destroy_level(Level *level);

#endif
