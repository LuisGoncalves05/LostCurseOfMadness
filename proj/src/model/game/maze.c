#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "maze.h"

struct Maze {
    uint8_t size_x, size_y;
    char** maze;
};

static void shuffle(int* arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}


static void dfs(Maze* maze, int x, int y) {
    maze->maze[y][x] = ' ';

    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};
    int dirs[] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dir = dirs[i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (new_x >= 0 && new_x <= maze->size_x - 1 && new_y >= 0 && new_y <= maze->size_y - 1 && maze->maze[new_y][new_x] == '#') { // Position inside the maze and there is a wall
            maze->maze[y + dy[dir] / 2][x + dx[dir] / 2] = ' ';  // Remove wall
            dfs(maze, new_x, new_y);
        }
    }
}

static int open_maze(Maze* maze, uint8_t percentage) {
    if (maze == NULL)
        return 1;
    
    uint16_t wall_area = 2 * (maze->size_x / 2 + 1) * (maze->size_y / 2 + 1) - 1; // 2 times O - 1 (V + E on spanning tree on a graph that passes through every other position)
    uint16_t visited = 0;
    uint16_t attempts = 0;

    uint16_t max_attempts = 255*255;
    while (visited < wall_area * percentage / 100.0 && attempts < max_attempts) {
        int random_x = rand() % maze->size_x;
        int random_y = rand() % maze->size_y;

        if (maze->maze[random_y][random_x] == '#') {
            maze->maze[random_y][random_x] = ' ';
            visited++;
        }

        attempts++;
    }
    
    return 0;
}

static int initialize_maze(Maze* maze, uint8_t size_x, uint8_t size_y) {
    if (maze == NULL)
        return 1;

    if ((size_x % 2 == 0) || (size_y % 2 == 0))
        return 1;
    
    maze->size_x = size_x;
    maze->size_y = size_y;

    char** mz = (char**) malloc(size_y * sizeof(char*));
    if (mz == NULL)
        return 1;

    for (uint16_t i = 0; i < size_y; i++) {
        mz[i] = (char*) malloc(size_x * sizeof(char));
        if (mz[i] == NULL) {
            for (uint16_t j = i-1; j >= 0; j--) {
                free(mz[j]);
            }
            free(mz);
            return 1;
        }
        memset(mz[i], '#', size_x);
    }
    mz[0][0] = ' ';
    maze->maze = mz;
    dfs(maze, 0, 0);

    return 0;
}

/* public functions */

Maze *create_maze(uint8_t size_x, uint8_t size_y) {
    srand(time(NULL));
    Maze* maze = (Maze *)malloc(sizeof(Maze));
    if (initialize_maze(maze, 31, 11)) return NULL;
    open_maze(maze, 30);
    print_maze(maze);

    return maze;
}

void free_maze(Maze* maze) {
    for (int i = 0; i < maze->size_y; i++) {
        free(maze->maze[i]);
    }
    free(maze->maze);
}

void print_maze(Maze* maze) {
    for (int j = 0; j < maze->size_y; j++) {
        for (int i = 0; i < maze->size_x; i++) {
            printf("%c", maze->maze[j][i]);
        }
        printf("\n");
    }
}
