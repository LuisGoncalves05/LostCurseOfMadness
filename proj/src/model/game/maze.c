#include "maze.h"
#include <stdlib.h>

struct Maze {
    int width;
    int height;
    uint8_t** cells;
};

// Layout do labirinto predefinido
static const uint8_t MAZE_LAYOUT[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,
    1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,
    1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,
    1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,
    1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,
    1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

Maze* create_maze(int width, int height) {
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    if (!maze) return NULL;

    maze->width = width;
    maze->height = height;

    // Aloca memória para a matriz do labirinto
    maze->cells = (uint8_t**)malloc(height * sizeof(uint8_t*));
    if (!maze->cells) {
        free(maze);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        maze->cells[i] = (uint8_t*)malloc(width * sizeof(uint8_t));
        if (!maze->cells[i]) {
            for (int j = 0; j < i; j++) {
                free(maze->cells[j]);
            }
            free(maze->cells);
            free(maze);
            return NULL;
        }

        // Inicializa o labirinto com o layout predefinido
        for (int j = 0; j < width && j < 31; j++) {
            if (i < 11) {
                maze->cells[i][j] = MAZE_LAYOUT[i * 31 + j];
            } else {
                maze->cells[i][j] = 0;  // Se estiver fora dos limites do layout, define como caminho
            }
        }
    }

    return maze;
}

void free_maze(Maze* maze) {
    if (!maze) return;

    if (maze->cells) {
        for (int i = 0; i < maze->height; i++) {
            free(maze->cells[i]);
        }
        free(maze->cells);
    }
    free(maze);
}

int draw_maze(Maze* maze, uint8_t* frame_buffer) {
    if (!maze || !frame_buffer) {
        printf("Error: Maze or frame buffer is NULL\n");
        return 1;
    }

    printf("Drawing maze: width=%d, height=%d\n", maze->width, maze->height);

    // Offset para centralizar o labirinto na tela
    int offset_x = (800 - (maze->width * CELL_SIZE)) / 2;  // Assumindo largura da tela de 800
    int offset_y = (600 - (maze->height * CELL_SIZE)) / 2; // Assumindo altura da tela de 600

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->cells[y][x] == 1) {
                // Calcula a posição na tela
                int screen_x = offset_x + (x * CELL_SIZE);
                int screen_y = offset_y + (y * CELL_SIZE);

                // Desenha a parede
                vga_draw_rectangle(screen_x, screen_y,
                                  CELL_SIZE, CELL_SIZE,
                                  WALL_COLOR, frame_buffer);
            }
        }
    }

    return 0;
}

bool check_collision(Maze* maze, int x, int y, int width, int height) {
    if (!maze) return false;

    // Offset para centralizar o labirinto na tela
    int offset_x = (800 - (maze->width * CELL_SIZE)) / 2;
    int offset_y = (600 - (maze->height * CELL_SIZE)) / 2;

    // Converte as coordenadas do pixel para coordenadas do labirinto
    int cell_x = (x - offset_x) / CELL_SIZE;
    int cell_y = (y - offset_y) / CELL_SIZE;

    // Adiciona margem para detectar colisões com mais precisão
    int end_cell_x = (x + width - offset_x) / CELL_SIZE;
    int end_cell_y = (y + height - offset_y) / CELL_SIZE;

    // Verifica se há colisão com alguma parede
    for (int check_y = cell_y; check_y <= end_cell_y; check_y++) {
        for (int check_x = cell_x; check_x <= end_cell_x; check_x++) {
            if (check_x >= 0 && check_x < maze->width &&
                check_y >= 0 && check_y < maze->height) {
                if (maze->cells[check_y][check_x] == 1) {
                    return true;  // Colisão detectada
                }
            }
        }
    }
    return false;  // Sem colisão
}
