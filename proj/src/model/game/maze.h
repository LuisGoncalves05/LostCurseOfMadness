#ifndef MAZE_H
#define MAZE_H

#include <lcom/lcf.h>

typedef struct Maze Maze;

Maze *create_maze(uint8_t size_x, uint8_t size_y);
void free_maze(Maze* maze);

void print_maze(Maze* maze);

#endif
