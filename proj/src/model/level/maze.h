#ifndef MAZE_H
#define MAZE_H

#include "../../drivers/video/gpu.h"
#include <lcom/lcf.h>

#define WALL_COLOR 0x07         // Cor cinza para as parede
#define CELL_SIZE 52            // Tamanho total da célula: espaço de 50px + largura da linha de 2px
#define WALL_WIDTH 2            // Largura das paredes em pixels
#define LINE_COLLISION_MARGIN 2 // Margem para detecção de colisão com as linhas
#define WIDTH 1024
#define HEIGHT 768

typedef struct Maze Maze;
typedef struct Line Line;

extern uint32_t frame_size;

Maze *create_maze(uint8_t width, uint8_t height);
void free_maze(Maze *maze);

int draw_maze(Maze *maze, uint8_t *frame_buffer);

bool check_collision(Maze *maze, int x, int y, int width, int height);
bool check_line_collision(int x, int y, int width, int height, Line line);

void init_maze_buffer();
void free_maze_buffer();

uint8_t get_width(Maze *maze);
uint8_t get_height(Maze *maze);
int get_line_count(Maze *maze);

#endif
