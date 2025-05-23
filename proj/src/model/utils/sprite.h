#ifndef SPRITE_H
#define SPRITE_H

#include "lcom/lcf.h"

typedef struct {
  int x, y;
  int width, height;
  int xspeed, yspeed;
  uint8_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t xpm, int x, int y, int xspeed, int yspeed);

void destroy_sprite(Sprite *sp);

#endif // SPRITE_H
