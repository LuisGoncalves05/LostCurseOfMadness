#include "bullet.h"

#include "drivers/video/gpu.h" // for draw_sprite_rotated()
#include "lcom/lcf.h"          // for xpm_map_t
#include <math.h>
#include <stdlib.h>
#include <math.h>

#include "drivers/video/gpu.h"

struct Bullet {
    Sprite *sprite;  /**< Underlying sprite */
    int     dx, dy;  /**< Velocity components */
    bool    active;  /**< Active flag */
};

extern uint16_t x_res, y_res;

Bullet *create_bullet(int x, int y, double angle) {
    Bullet *bullet = malloc(sizeof(Bullet));
    if (!bullet) return NULL;

    bullet->sprite = create_sprite((xpm_map_t) cross, x, y, BULLET_SPEED, BULLET_SPEED);
    bullet->dx = (int)(cos(angle) * BULLET_SPEED);
    bullet->dy = (int)(sin(angle) * BULLET_SPEED);
    bullet->active = true;

    return bullet;
}

void update_bullet(Bullet *bullet, Maze *maze) {
    if (!bullet->active) return;
    bullet->sprite->x += bullet->dx;
    bullet->sprite->y += bullet->dy;

    if (bullet->sprite->x < 0 || bullet->sprite->x > x_res ||
        bullet->sprite->y < 0 || bullet->sprite->y > y_res) {
        bullet->active = false;
    }

    if(check_rectangle_line_collision(maze, bullet->sprite->x,bullet->sprite->y, bullet->sprite->width, bullet->sprite->height)) {
        bullet->active = false; // Deactivate if it hits a wall
    }
}

void destroy_bullet(Bullet *bullet) {
    if (!bullet) return;
    destroy_sprite(bullet->sprite);
    free(bullet);
}

bool bullet_is_active(Bullet *bullet) {
    if (!bullet) return false;
    return bullet->active;
}

void draw_bullet(Bullet *bullet, uint8_t *frame_buffer) {
    if (!bullet) return;
    draw_sprite(bullet->sprite, frame_buffer);
}
