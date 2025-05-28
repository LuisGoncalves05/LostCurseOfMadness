#include "maze.h"
#include <stdlib.h>

struct Maze {
    uint8_t width;
    uint8_t height;
    maze_entity **cells;
    uint8_t mob_count;
};

/* Create and destroy section */

static void(shuffle)(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

static void(dfs)(Maze *maze, int x, int y) {
    maze->cells[y][x] = EMPTY;

    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};
    int dirs[] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dir = dirs[i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (new_x > 0 && new_x < maze->width - 1 && new_y > 0 && new_y < maze->height - 1 && maze->cells[new_y][new_x] == WALL) { // Position inside the maze and there is a wall
            maze->cells[y + dy[dir] / 2][x + dx[dir] / 2] = EMPTY;                                                                // Remove wall
            dfs(maze, new_x, new_y);
        }
    }
}

static int(initialize_maze)(Maze *maze, uint8_t width, uint8_t height) {
    if (maze == NULL)
        return 1;

    if ((width % 2 == 0) || (height % 2 == 0))
        return 1;


    maze->width = width > x_res / CELL_SIZE ? x_res / CELL_SIZE : width;
    maze->height = height > y_res / CELL_SIZE ? y_res / CELL_SIZE : height;

    maze_entity **mz = (maze_entity **) malloc(height * sizeof(maze_entity *));
    if (mz == NULL)
        return 1;

    for (uint16_t i = 0; i < height; i++) {
        mz[i] = (maze_entity *) malloc(width * sizeof(maze_entity));

        if (mz[i] == NULL) {
            for (int16_t j = i - 1; j >= 0; j--) {
                free(mz[j]);
            }
            free(mz);
            return 1;
        }
        for (int j = 0; j < width; j++)
            mz[i][j] = WALL;
    }
    maze->cells = mz;
    dfs(maze, 1, 1);
    mz[maze->height - 2][maze->width - 2] = WIN;

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

static int(open_maze)(Maze *maze, uint8_t percentage) {
    if (maze == NULL)
        return 1;

    uint16_t wall_area = 2 * (maze->width / 2 + 1) * (maze->height / 2 + 1) - 1; // 2 times O - 1 (V + E on spanning tree on a graph that passes through every other position)
    uint16_t visited = 0;
    uint16_t attempts = 0;

    uint16_t max_attempts = 255 * 255;
    while (visited < wall_area * percentage / 100.0 && attempts < max_attempts) {
        int random_x = 1 + rand() % (maze->width - 2);  // Leave borders untouched
        int random_y = 1 + rand() % (maze->height - 2); // Leave borders untouched

        if (maze->cells[random_y][random_x] == WALL) {
            maze->cells[random_y][random_x] = EMPTY;
            visited++;
        }

        attempts++;
    }

    return 0;
}

Maze *(create_maze) (uint8_t width, uint8_t height, uint8_t mob_count) {
    srand(time(NULL));
    Maze *maze = (Maze *) malloc(sizeof(Maze));
    if (!maze)
        return NULL;

    if (initialize_maze(maze, width, height)) {
        free(maze);
        return NULL;
    }

    open_maze(maze, MAZE_OPENING_PERCENTAGE);
    generate_mob_positions(maze, mob_count);

    return maze;
}

void destroy_maze(Maze *maze) {
    if (!maze)
        return;

    if (maze->cells) {
        for (int i = 0; i < maze->height; i++) {
            free(maze->cells[i]);
        }
        free(maze->cells);
    }

    free(maze);
}

/* Getter and setter section */

uint8_t(get_width)(Maze *maze) {
    return maze->width;
}

uint8_t(get_height)(Maze *maze) {
    return maze->height;
}

uint8_t(get_mob_count)(Maze *maze) {
    return maze->mob_count;
}

void set_mob_count(Maze *maze, uint8_t mob_count) {
    maze->mob_count = mob_count;
}

/* Statics section */

/* Others section */

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

bool(check_rectangle_collision)(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
    return !(
        x1 + width1 < x2 ||  // a to the left of b
        x1 > x2 + width2 ||  // b to the left of a
        y1 + height1 < y2 || // in normal people coordinates, a above b
        y1 > y2 + height2    // in normal people coordinates, b above a
    );
}

bool(check_sprite_collision)(Sprite *a, Sprite *b) {
    if (!a || !b) {
        printf("check_sprite_collision: NULL provided");
        return false;
    }

    return check_rectangle_collision(a->x, a->y, a->width, a->height, b->x, b->y, b->width, b->height);
}

bool(check_wall_collision)(Maze *maze, Sprite *sprite) {
    if (!maze)
        return false;

    double scaled_down_x = sprite->x / CELL_SIZE;
    double scaled_down_y = sprite->y / CELL_SIZE;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x = scaled_down_x + i;
            int y = scaled_down_y + j;
            if (x >= 0 && x < x_res &&
                y >= 0 && y < y_res &&
                maze->cells[y][x] == WALL) {
                if (check_rectangle_collision(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, sprite->x, sprite->y, sprite->width, sprite->height)) {
                    return true;
                }
            }
        }
    }

    return false;
}

/* Draw section */

int(draw_maze)(Maze *maze, uint8_t *frame_buffer) {
    if (!maze || !frame_buffer) {
        printf("Error: Maze or frame buffer is NULL\n");
        return 1;
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->cells[y][x] == WALL) {
                vga_draw_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, WALL_COLOR, secondary_frame_buffer);
            } else if (maze->cells[y][x] == WIN) {
                vga_draw_rectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, WIN_COLOR, secondary_frame_buffer);
            }
        }
    }

    return 0;
}
