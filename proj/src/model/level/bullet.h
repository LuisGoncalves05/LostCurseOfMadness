#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <stdint.h>
#include "model/utils/sprite.h"
#include "model/level/maze.h"

#define BULLET_SPEED   6   /**< Speed in pixels per tick */
#define MAX_BULLETS   50   /**< Maximum simultaneous bullets */

/**
 * @brief A single bullet instance.
 */
typedef struct {
    Sprite *sprite;  /**< Underlying sprite */
    int     dx, dy;  /**< Velocity components */
    bool    active;  /**< Active flag */
} Bullet;

/** Global array of active bullets */
extern Bullet *bullets[MAX_BULLETS];
/** Current bullet count */
extern int bullet_count;

/**
 * @brief Spawn a new bullet at (x,y) moving at given angle.
 * @param x      Initial x-coordinate (in pixels)
 * @param y      Initial y-coordinate (in pixels)
 * @param angle  Direction in radians
 * @return       Pointer to the created Bullet, or NULL on failure.
 */
Bullet *create_bullet(int x, int y, double angle);

/**
 * @brief Update (move & deactivate off-screen) a single bullet.
 * @param b  Pointer to the Bullet to update.
 */
void update_bullet(Bullet *b, Maze *maze);

/**
 * @brief Advance **all** bullets: move, deactivate off-screen, and free them.
 */
void update_all_bullets(Maze *maze);

/**
 * @brief Draw **all** active bullets, rotated by given angle.
 * @param frame_buffer  Pointer to the target frame buffer.
 * @param theta         Rotation angle (radians) to apply when drawing.
 */
void draw_all_bullets(uint8_t *frame_buffer, double theta);

/**
 * @brief Destroy (free) a single bullet.
 * @param b  Pointer to the Bullet to destroy.
 */
void destroy_bullet(Bullet *b);

/**
 * @brief Destroy **all** bullets and reset count to zero.
 */
void reset_bullets(void);

#endif /* BULLET_H */
