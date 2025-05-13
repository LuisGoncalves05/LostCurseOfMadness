#include "level.h"

#include <time.h>

extern Sprite *player_sprite;

Level *create_level(uint8_t number) {
    Level *level = (Level *)malloc(sizeof(Level));
    if (!level) return NULL;

    level->number = number;
    level->maze = (Maze *)malloc(sizeof(Maze));
    srand(time(NULL));
    Maze* maze = malloc(sizeof(Maze));
    if (initialize_maze(maze, 31, 11)) {
        free(level);
        return NULL;
    }
    open_maze(maze, 30);
    print_maze(maze);
    if (!level->maze) {
        free(level);
        return NULL;
    }

    level->player = create_player(HEALTH, player_sprite);
    if (!level->player) {
        free(level->maze);
        free(level);
        return NULL;
    }

    return level;
}

void destroy_level(Level *level) {
    if (level) {
        destroy_player(level->player);
        free(level->maze);
        free(level);
    }
}
