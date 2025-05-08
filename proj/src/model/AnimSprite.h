#ifndef ANIM_SPRITE_H
#define ANIM_SPRITE_H

#include "sprite.h" // Depend on Sprite

// Animated Sprite structure
typedef struct {
    Sprite *sp;         // Pointer to the standard sprite
    int aspeed;         // Frames per pixmap
    int cur_aspeed;     // Frames left to next change
    int num_fig;        // Total number of pixmaps
    int cur_fig;        // Index of current pixmap
    char **map;         // Array of pixmaps
} AnimSprite;

// AnimSprite functions
AnimSprite *create_animSprite(uint8_t no_pic, char ***pics);
void destroy_anim_sprite(AnimSprite *anim);


#endif 
