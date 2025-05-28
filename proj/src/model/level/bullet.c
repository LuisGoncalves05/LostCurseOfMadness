#include <math.h>
#include <stdlib.h>

#include "bullet.h"
#include "drivers/video/gpu.h"

struct Bullet {
    Sprite *sprite; /**< Underlying sprite */
    int dx, dy;     /**< Velocity components */
    bool active;    /**< Active flag */
};

/* Create and destroy section */

Bullet *create_bullet(int x, int y, double angle) {
    Bullet *bullet = malloc(sizeof(Bullet));
    if (!bullet)
        return NULL;

    bullet->sprite = create_sprite((xpm_map_t) bullet_xpm, x, y, BULLET_SPEED, BULLET_SPEED);
    bullet->dx = (int) (cos(angle) * BULLET_SPEED);
    bullet->dy = (int) (sin(angle) * BULLET_SPEED);
    bullet->active = true;

    return bullet;
}

void destroy_bullet(Bullet *bullet) {
    if (!bullet)
        return;
    destroy_sprite(bullet->sprite);
    free(bullet);
}

/* Getter and setter section */

Sprite *bullet_get_sprite(Bullet *bullet) {
    return bullet->sprite;
}

int bullet_get_dx(Bullet *bullet) {
    return bullet->dx;
}

int bullet_get_dy(Bullet *bullet) {
    return bullet->dy;
}

bool bullet_get_active(Bullet *bullet) {
    return bullet->active;
}

void bullet_set_active(Bullet *bullet, bool active) {
    bullet->active = active;
}

/* Statics section */

/* Others section */

/* Draw section */

void draw_bullet(Bullet *bullet, uint8_t *frame_buffer) {
    if (!bullet)
        return;
    draw_transparent_sprite(bullet->sprite, frame_buffer);
}
