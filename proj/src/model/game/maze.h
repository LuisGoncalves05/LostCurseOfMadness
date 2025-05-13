#ifndef MAZE_H
#define MAZE_H

#include <lcom/lcf.h>

typedef struct {
    uint8_t size_x, size_y;
    char** maze;
} Maze;

int initialize_maze(Maze* maze, uint8_t size_x, uint8_t size_y);

void free_maze(Maze* maze);

void print_maze(Maze* maze);

int open_maze(Maze* maze, uint8_t percentage);

void dfs(Maze* maze, int x, int y);

void shuffle(int* arr, int n);

#endif
