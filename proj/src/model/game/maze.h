#ifndef MAZE_H
#define MAZE_H

#include <lcom/lcf.h>
#include "../../controller/video/gpu.h"

#define WALL_COLOR 0x07  // Cor cinza para as paredes
#define CELL_SIZE 52     // Tamanho total da célula: espaço de 50px + largura da linha de 2px
#define WALL_WIDTH 2     // Largura das paredes em pixels
#define LINE_COLLISION_MARGIN 2  // Margem para detecção de colisão com as linhas
#define WIDTH 1024
#define HEIGHT 768

typedef struct Maze Maze;
typedef struct Line Line;

// Estrutura para representar uma linha no labirinto
struct Line {
    int x1, y1;  // Ponto inicial 
    int x2, y2;  // Ponto final
};

Maze *create_maze(uint8_t width, uint8_t height);
void free_maze(Maze* maze);
int draw_maze(Maze* maze, uint8_t* frame_buffer);
bool check_collision(Maze* maze, int x, int y, int width, int height);

// Função para desenhar uma única linha
void draw_line(int x1, int y1, int x2, int y2, uint32_t color, uint8_t* frame_buffer);

// Função auxiliar para verificar colisão com uma linha
bool check_line_collision(int x, int y, int width, int height, Line line);

#endif
