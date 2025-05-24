#include "level.h"

struct Level {
  uint8_t number;
  Maze *maze;
  Player *player;
  double delta; // atan2 of the player - mouse_position
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
  if (!level) return NULL;
  return level->player;
}

Maze *get_maze(Level *level) {
  if (!level) return NULL;
  return level->maze;
}

double get_delta(Level* level) {
  if (!level) return 0;
  return level->delta;
}

void update_delta(Level *level, double mouse_x, double mouse_y) {
  Sprite *player_sprite = get_sprite(level->player);
  double player_center_x = player_sprite->x + player_sprite->width / 2.0;
  double player_center_y = player_sprite->y + player_sprite->height / 2.0;

  double dx = mouse_x - player_center_x;
  double dy = mouse_y - player_center_y;

  level->delta = atan2(dy, dx);
}
