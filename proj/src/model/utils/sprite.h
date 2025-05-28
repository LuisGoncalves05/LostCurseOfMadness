#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>

/**
 * @brief Represents a Sprite used for rendering objects on screen.
 */
typedef struct {
    int x, y;           /**< X and Y position of the sprite */
    int width, height;  /**< Dimensions of the sprite */
    double xspeed, yspeed; /**< Velocity in the X and Y direction */
    uint8_t *map;       /**< Pointer to the sprite's pixel data */
} Sprite;

/**
 * @brief Creates a new sprite with the given XPM image, position, and speed.
 *
 * Does not draw the sprite on the screen.
 *
 * @param xpm XPM image to use for the sprite.
 * @param x Initial X position.
 * @param y Initial Y position.
 * @param xspeed Speed in the X direction.
 * @param yspeed Speed in the Y direction.
 * @return Pointer to the created Sprite, or NULL on failure.
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y, double xspeed, double yspeed);

/**
 * @brief Frees the memory associated with the sprite.
 *
 * @param sp Pointer to the Sprite structure to destroy.
 */
void destroy_sprite(Sprite *sp);

#endif /* SPRITE_H */
