#include "level.h"

struct Level {
  uint8_t number;
  Maze *maze;
  Player *player;
  Mob **mobs;
};

Level *create_level(uint8_t number) {
  Level *level = (Level *) malloc(sizeof(Level));
  if (!level)
    return NULL;

  level->number = number;
  uint8_t mob_count = 5 * number;
  level->maze = create_maze(30 * number + 1, 30 * number + 1, mob_count);
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

  level->mobs = malloc(sizeof(Mob *) * mob_count);
  if (!level->mobs) {
    free(level->maze);
    free(level->player);
    free(level);
    return NULL;
  }

  Point **mob_positions = get_mob_positions(level->maze);

  for (int i = 0; i < mob_count; i++) {
    Point *position = mob_positions[i];    
    Sprite *mob_sprite = create_sprite((xpm_map_t) cross, position->x*10, position->y*10, 0 , 0);
    level->mobs[i] = create_mob(mob_sprite);
    if (!level->mobs[i]) {
      for (int j = i-1; j >= 0; j--) {
        destroy_mob(level->mobs[j]);
        free(level->maze);
        free(level->player);
        free(level);
        return NULL;
      }
    }
  }

  return level;
}

void destroy_level(Level *level) {
  if (level != NULL) {
    destroy_player(level->player);
    free_maze(level->maze);
    for (int i = 0; i < get_mob_count(get_maze(level)); i++) {
      //destroy_mob(level->mobs[i]);
    }
    free(level);
  }
}

Player *get_player(Level *level) {
  return level->player;
}

Maze *get_maze(Level *level) {
  return level->maze;
}

Mob **get_mobs(Level *level) {
  return level->mobs;
}
