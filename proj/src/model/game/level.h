#ifndef LEVEL_H
#define LEVEL_H

#include "maze.h"
#include "player.h"

typedef struct {
  uint8_t number;
  Maze* maze;
  Player* player;
} Level;

#endif
