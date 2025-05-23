#ifndef LEVEL_H
#define LEVEL_H

#include <lcom/lcf.h>

#include "maze.h"
#include "player.h"

typedef struct Level Level;

Level *create_level(uint8_t number);
void destroy_level(Level *level);

struct Player *get_player(Level *level);
Maze *get_maze(Level *level);


#endif
