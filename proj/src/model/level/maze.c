#include "maze.h"
#include <stdlib.h>

struct Maze {
    uint8_t width;
    uint8_t height;
    uint8_t **cells;
    uint8_t mob_count;
    Line *lines;
    int line_count;
};

uint8_t *maze_buffer = NULL;

static void(add_line)(Maze *maze, int x1, int y1, int x2, int y2, int *line_index) {
    if (*line_index >= maze->line_count) {
        maze->line_count *= 2;
        maze->lines = (Line *) realloc(maze->lines, maze->line_count * sizeof(Line));
        if (!maze->lines) {
            printf("Erro ao realocar memória para linhas\n");
            return;
        }
    }

    maze->lines[*line_index].x1 = x1;
    maze->lines[*line_index].y1 = y1;
    maze->lines[*line_index].x2 = x2;
    maze->lines[*line_index].y2 = y2;
    (*line_index)++;
}

bool(check_line_collision)(int x, int y, int width, int height, Line line) {
    // Retângulo do objeto
    int left = x;
    int right = x + width;
    int top = y;
    int bottom = y + height;

    // Expandir a linha com a margem de colisão
    int margin = LINE_COLLISION_MARGIN;

    // Verifica se a linha está completamente fora do retângulo
    if ((line.x1 < left - margin && line.x2 < left - margin) ||
        (line.x1 > right + margin && line.x2 > right + margin) ||
        (line.y1 < top - margin && line.y2 < top - margin) ||
        (line.y1 > bottom + margin && line.y2 > bottom + margin)) {
        return false;
    }

    // Algoritmo de interseção linha-retângulo usando o algoritmo Cohen-Sutherland modificado
    // Calcula os parâmetros da equação da linha: y = mx + b
    double dx = line.x2 - line.x1;
    double dy = line.y2 - line.y1;

    // Se a linha é vertical (caso especial)
    if (dx == 0) {
        if (line.x1 >= left - margin && line.x1 <= right + margin) {
            // Verifica se a linha cruza o retângulo verticalmente
            int min_y = (line.y1 < line.y2) ? line.y1 : line.y2;
            int max_y = (line.y1 > line.y2) ? line.y1 : line.y2;
            return !(max_y < top - margin || min_y > bottom + margin);
        }
        return false;
    }

    // Se a linha é horizontal (caso especial)
    if (dy == 0) {
        if (line.y1 >= top - margin && line.y1 <= bottom + margin) {
            // Verifica se a linha cruza o retângulo horizontalmente
            int min_x = (line.x1 < line.x2) ? line.x1 : line.x2;
            int max_x = (line.x1 > line.x2) ? line.x1 : line.x2;
            return !(max_x < left - margin || min_x > right + margin);
        }
        return false;
    }

    // Linha diagonal - verificar interseção com cada lado do retângulo
    double m = dy / dx;
    double b = line.y1 - m * line.x1;

    // Expandir o retângulo com a margem
    left -= margin;
    right += margin;
    top -= margin;
    bottom += margin;

    // Verificar interseção com cada lado do retângulo expandido
    // Lado esquerdo: x = left
    double y_left = m * left + b;
    if (y_left >= top && y_left <= bottom &&
        ((line.x1 <= left && line.x2 >= left) || (line.x1 >= left && line.x2 <= left))) {
        return true;
    }

    // Lado direito: x = right
    double y_right = m * right + b;
    if (y_right >= top && y_right <= bottom &&
        ((line.x1 <= right && line.x2 >= right) || (line.x1 >= right && line.x2 <= right))) {
        return true;
    }

    // Lado superior: y = top
    double x_top = (top - b) / m;
    if (x_top >= left && x_top <= right &&
        ((line.y1 <= top && line.y2 >= top) || (line.y1 >= top && line.y2 <= top))) {
        return true;
    }

    // Lado inferior: y = bottom
    double x_bottom = (bottom - b) / m;
    if (x_bottom >= left && x_bottom <= right &&
        ((line.y1 <= bottom && line.y2 >= bottom) || (line.y1 >= bottom && line.y2 <= bottom))) {
        return true;
    }

    return false;
}

bool(check_sprite_collision)(Sprite *a, Sprite *b) {
    if (!a || !b) {
        printf("check_sprite_collision: NULL provided");
        return false;
    }

    return !(
        a->x + a->width < b->x ||  // a to the left of b
        a->x > b->x + b->width ||  // b to the left of a
        a->y + a->height < b->y || // in normal people coordinates, a above b
        a->y > b->y + b->height    // in normal people coordinates, b above a
    );
}

static void(shuffle)(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

static void(dfs)(Maze *maze, int x, int y) {
    maze->cells[y][x] = 0;

    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};
    int dirs[] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dir = dirs[i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (new_x >= 0 && new_x <= maze->width - 1 && new_y >= 0 && new_y <= maze->height - 1 && maze->cells[new_y][new_x] == 1) { // Position inside the maze and there is a wall
            maze->cells[y + dy[dir] / 2][x + dx[dir] / 2] = 0;                                                                     // Remove wall
            dfs(maze, new_x, new_y);
        }
    }
}

static int(open_maze)(Maze *maze, uint8_t percentage) {
    if (maze == NULL)
        return 1;

    uint16_t wall_area = 2 * (maze->width / 2 + 1) * (maze->height / 2 + 1) - 1; // 2 times O - 1 (V + E on spanning tree on a graph that passes through every other position)
    uint16_t visited = 0;
    uint16_t attempts = 0;

    uint16_t max_attempts = 255 * 255;
    while (visited < wall_area * percentage / 100.0 && attempts < max_attempts) {
        int random_x = rand() % maze->width;
        int random_y = rand() % maze->height;

        if (maze->cells[random_y][random_x] == WALL) {
            maze->cells[random_y][random_x] = EMPTY;
            visited++;
        }

        attempts++;
    }

    return 0;
}

static int(initialize_maze)(Maze *maze, uint8_t width, uint8_t height) {
    if (maze == NULL)
        return 1;

    if ((width % 2 == 0) || (height % 2 == 0))
        return 1;

    maze->width = width;
    maze->height = height;

    uint8_t **mz = (uint8_t **) malloc(height * sizeof(uint8_t *));
    if (mz == NULL)
        return 1;

    for (uint16_t i = 0; i < height; i++) {
        mz[i] = (uint8_t *) malloc(width * sizeof(uint8_t));

        if (mz[i] == NULL) {
            for (int16_t j = i - 1; j >= 0; j--) {
                free(mz[j]);
            }
            free(mz);
            return 1;
        }
        memset(mz[i], WALL, width);
    }
    maze->cells = mz;
    mz[0][0] = EMPTY;
    dfs(maze, 0, 0);

    return 0;
}

static int(generate_mob_positions)(Maze *maze, uint8_t mob_count) {
    int positions = 0;
    while (positions < mob_count) {
        int i = rand() % maze->width;
        int j = rand() % maze->height;
        if (i != 0 && j != 0 && maze->cells[j][i] == EMPTY) {
            positions++;
            maze->cells[j][i] = MOB;
        }
    }
    maze->mob_count = mob_count;

    return 0;
}

/* public functions */
Maze *(create_maze) (uint8_t width, uint8_t height, uint8_t mob_count) {
    srand(time(NULL));
    Maze *maze = (Maze *) malloc(sizeof(Maze));
    if (!maze)
        return NULL;

    if (initialize_maze(maze, width, height)) {
        free(maze);
        return NULL;
    }

    open_maze(maze, 30);
    generate_mob_positions(maze, mob_count);

    // Estimar o número máximo de linhas que podemos precisar
    // No pior caso, cada célula de parede cria 4 linhas
    int max_lines = maze->width * maze->height * 2;
    maze->line_count = max_lines;
    maze->lines = (Line *) malloc(max_lines * sizeof(Line));
    if (!maze->lines) {
        printf("Erro ao alocar memória para linhas\n");
        free_maze(maze);
        return NULL;
    }

    int line_index = 0;

    // Converter a matriz de células para um conjunto de linhas
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->cells[y][x] == 1) {
                // Verificar se há paredes adjacentes e criar linhas

                // Verificar parede à direita
                if (x < maze->width - 1 && maze->cells[y][x + 1] == 1) {
                    add_line(maze, x, y, x + 1, y, &line_index);
                }

                // Verificar parede abaixo
                if (y < maze->height - 1 && maze->cells[y + 1][x] == 1) {
                    add_line(maze, x, y, x, y + 1, &line_index);
                }

                // Verificar parede à esquerda (se não houver ligação)
                if (x > 0 && maze->cells[y][x - 1] != 1) {
                    // Linha isolada - criar um ponto
                    add_line(maze, x, y, x, y, &line_index);
                }

                // Verificar parede acima (se não houver ligação)
                if (y > 0 && maze->cells[y - 1][x] != 1 &&
                    !(x > 0 && maze->cells[y][x - 1] == 1)) {
                    // Linha isolada - criar um ponto
                    add_line(maze, x, y, x, y, &line_index);
                }
            }
        }
    }

    // Atualizar o número real de linhas
    maze->line_count = line_index;

    return maze;
}

void(free_maze)(Maze *maze) {
    if (!maze)
        return;

    if (maze->cells) {
        for (int i = 0; i < maze->height; i++) {
            free(maze->cells[i]);
        }
        free(maze->cells);
    }

    if (maze->lines) {
        free(maze->lines);
    }

    free(maze);
}

int(draw_maze)(Maze *maze, uint8_t *frame_buffer) {
    if (!maze || !frame_buffer) {
        printf("Error: Maze or frame buffer is NULL\n");
        return 1;
    }

    // Offset para centralizar o labirinto na tela
    int offset_x = (WIDTH - (maze->width * CELL_SIZE)) / 2;   // Assumindo largura da tela de WIDTH
    int offset_y = (HEIGHT - (maze->height * CELL_SIZE)) / 2; // Assumindo altura da tela de HEIGHT

    // Desenhar cada linha do labirinto
    for (int i = 0; i < maze->line_count; i++) {
        int x1 = offset_x + maze->lines[i].x1 * CELL_SIZE;
        int y1 = offset_y + maze->lines[i].y1 * CELL_SIZE;
        int x2 = offset_x + maze->lines[i].x2 * CELL_SIZE;
        int y2 = offset_y + maze->lines[i].y2 * CELL_SIZE;

        // Desenhar uma linha grossa (2 pixels de largura)
        if (x1 == x2 && y1 == y2) {
            // Ponto isolado - desenhar como um quadrado pequeno
            vga_draw_rectangle(x1, y1, WALL_WIDTH, WALL_WIDTH, WALL_COLOR, frame_buffer);
        } else {
            // Linha - usar algoritmo de Bresenham com espessura 2
            vga_draw_line(x1, y1, x2, y2, WALL_COLOR, frame_buffer);

            // Desenhar linha paralela para criar espessura
            if (x1 == x2) {
                // Linha vertical
                vga_draw_line(x1 + 1, y1, x2 + 1, y2, WALL_COLOR, frame_buffer);
            } else if (y1 == y2) {
                // Linha horizontal
                vga_draw_line(x1, y1 + 1, x2, y2 + 1, WALL_COLOR, frame_buffer);
            } else {
                // Linha diagonal - não tratamos aqui, mas poderia ser implementado
                // para garantir consistência visual
            }
        }
    }

    return 0;
}

bool(check_rectangle_line_collision)(Maze *maze, int x, int y, int width, int height) {
    if (!maze)
        return false;

    // Verificar colisão com todas as linhas do labirinto
    for (int i = 0; i < maze->line_count; i++) {
        // Converter coordenadas das linhas para coordenadas da tela
        int offset_x = (WIDTH - (maze->width * CELL_SIZE)) / 2;
        int offset_y = (HEIGHT - (maze->height * CELL_SIZE)) / 2;

        Line line;
        line.x1 = offset_x + maze->lines[i].x1 * CELL_SIZE;
        line.y1 = offset_y + maze->lines[i].y1 * CELL_SIZE;
        line.x2 = offset_x + maze->lines[i].x2 * CELL_SIZE;
        line.y2 = offset_y + maze->lines[i].y2 * CELL_SIZE;

        if (check_line_collision(x, y, width, height, line)) {
            return true;
        }
    }

    return false;
}

void(init_maze_buffer)() {
    if (maze_buffer == NULL) {
        maze_buffer = (uint8_t *) malloc(frame_size);
        if (maze_buffer == NULL) {
            printf("Erro ao alocar maze_buffer\n");
        }
    }
}

void(free_maze_buffer)() {
    if (maze_buffer != NULL) {
        free(maze_buffer);
        maze_buffer = NULL;
    }
}

uint8_t(get_width)(Maze *maze) {
    return maze->width;
}

uint8_t(get_height)(Maze *maze) {
    return maze->width;
}

int(get_line_count)(Maze *maze) {
    return maze->line_count;
}

Point **(get_mob_positions) (Maze *maze) {
    int point_no = 0;
    Point **points = malloc(sizeof(Point *) * maze->mob_count);

    if (!points)
        return NULL;

    for (int j = 0; j < maze->height; j++) {
        for (int i = 0; i < maze->width; i++) {
            if (maze->cells[j][i] == MOB) {
                Point *p = malloc(sizeof(Point));
                p->x = i;
                p->y = j;
                points[point_no] = p;
                point_no++;
            }
        }
    }

    return points;
}

uint8_t(get_mob_count)(Maze *maze) {
    return maze->mob_count;
}

void set_mob_count(Maze *maze, uint8_t mob_count) {
    maze->mob_count = mob_count;
}
