#include "level.h"

struct Level {
  uint8_t number;
  Maze *maze;
  Player *player;
};

Level *create_level(uint8_t number) {
  Level *level = (Level *) malloc(sizeof(Level));
  if (!level)
    return NULL;

  level->number = number;
  level->maze = create_maze(31, 11);
  if (!level->maze) {
    free(level);
    return NULL;
  }

  level->player = create_player();
  if (!level->player) {
    free(level->maze);
    free(level);
    return NULL;
  }

  return level;
}

void destroy_level(Level *level) {
  if (level != NULL) {
    destroy_player(level->player);
    free_maze(level->maze);
    free(level);
  }
}

Player *get_player(Level *level) {
  return level->player;
}

Maze *get_maze(Level *level) {
  return level->maze;
}
