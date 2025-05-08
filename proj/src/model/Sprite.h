#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

// Basic Sprite structure
typedef struct {
    int x, y;           // Current position
    int width, height;  // Dimensions
    int xspeed, yspeed; // Current speed
    char *map;          // The pixmap
} Sprite;

// Sprite function declarations
Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);
void destroy_sprite(Sprite *sp);


#endif // SPRITE_H
