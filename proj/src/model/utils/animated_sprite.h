#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <lcom/lcf.h>
#include <stdarg.h>
#include "model/utils/sprite.h"

typedef struct {
    Sprite *sprite;
    uint32_t frames_per_sprite;
    uint32_t current_frame;
    uint32_t number_sprites;
    uint32_t current_sprite;
    uint8_t **maps;
} AnimatedSprite;

AnimatedSprite *create_animated_sprite(Sprite *sprite, uint32_t frames_per_sprite, uint32_t number_sprites, ...);

int destroy_animated_sprite(AnimatedSprite *anim_sprite);

#endif
