#include "level.h"

struct Level {
    uint8_t number;
    Maze* maze;
    Player* player;
};

Level *create_level(uint8_t number) {
    Level *level = (Level *)malloc(sizeof(Level));
    if (!level) return NULL;

    level->number = number;
    level->maze = create_maze(31, 11);
    if (!level->maze) {
        free(level);
        return NULL;
    }

    Sprite *player_sprite = create_sprite((xpm_map_t) cross, 400, 400, 3, 3);
    level->player = create_player(player_sprite);
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
        free_maze(level->maze);
        free(level);
    }
}

struct Player *get_player(Level *level) {
    return level->player;
}

Maze *get_maze(Level *level){return level->maze;}
