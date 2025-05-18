#ifndef MAZE_H
#define MAZE_H

#include <lcom/lcf.h>
#include "../../controller/video/gpu.h"

#define WALL_COLOR 0x07  // Cor cinza para as paredes
#define CELL_SIZE 48     // Tamanho de cada célula do labirinto


typedef struct Maze Maze;

Maze* create_maze(int width, int height);
void free_maze(Maze* maze);
int draw_maze(Maze* maze, uint8_t* frame_buffer);
bool check_collision(Maze* maze, int x, int y, int width, int height);


#endif
