#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "model/utils/sprite.h"
#include <stdarg.h>

extern uint16_t x_res, y_res;

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

int draw_animated_sprite(AnimatedSprite *sprite, uint8_t *frame_buffer);

#endif
