#include "level.h"

extern Sprite *player_sprite;

Level *create_level(uint8_t number) {
    Level *level = (Level *)malloc(sizeof(Level));
    if (!level) return NULL;

    level->number = number;
    level->maze = create_maze(31, 11);
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
        free_maze(level->maze);
        free(level);
    }
}
