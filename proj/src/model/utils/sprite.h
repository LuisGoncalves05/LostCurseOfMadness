#ifndef SPRITE_H
#define SPRITE_H

#include "../../drivers/video/gpu.h"

extern uint16_t x_res, y_res;

/**
 * @brief Represents a Sprite used for rendering objects on screen.
 */
typedef struct {
    int16_t x;       /**< X-coordinate of the sprite's top-left corner */
    int16_t y;       /**< Y-coordinate of the sprite's top-left corner */
    uint16_t width;  /**< Width of the sprite in pixels */
    uint16_t height; /**< Height of the sprite in pixels */
    double xspeed;   /**< Horizontal speed (pixels per frame or unit time) */
    double yspeed;   /**< Vertical speed (pixels per frame or unit time) */
    uint8_t *map;    /**< Pointer to the sprite's pixel data (e.g., XPM image) */
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
Sprite *create_sprite(xpm_map_t xpm, int16_t x, int16_t y, double xspeed, double yspeed);

/**
 * @brief Frees the memory associated with the sprite.
 *
 * @param sp Pointer to the Sprite structure to destroy.
 */
void destroy_sprite(Sprite *sp);

int draw_sprite(Sprite *sprite, uint8_t *frame_buffer);

int(draw_transparent_sprite)(Sprite *sprite, uint8_t *frame_buffer);

#endif /* SPRITE_H */
