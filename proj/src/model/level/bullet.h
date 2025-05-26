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
typedef struct Bullet Bullet;

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
 * @param bullet  Pointer to the Bullet to update.
 */
void update_bullet(Bullet *bullet, Maze *maze);

/**
 * @brief Destroy (free) a single bullet.
 * @param bullet Pointer to the Bullet to destroy.
 */
void destroy_bullet(Bullet *bullet);

bool bullet_is_active(Bullet *bullet);

void draw_bullet(Bullet *bullet, uint8_t *frame_buffer);

#endif /* BULLET_H */
