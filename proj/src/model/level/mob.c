#include "mob.h"

struct Mob {
    unsigned char health;
    int max_speed;
    AnimatedSprite *animated_sprite;
    MobState state;
    bool direction; // true for right, false for left
};

/* Create and destroy section */

Mob *create_mob(uint16_t x, uint16_t y) {
    Mob *mob = (Mob *) malloc(sizeof(Mob));
    if (!mob)
        return NULL;
    
    mob->health = MOB_HEALTH;
    int rx = rand() % 32;
    int ry = rand() % 32;
    Sprite *new_sprite = create_sprite((xpm_map_t) mob_idle_1, x + rx, y + ry, 0, 0);
    mob->animated_sprite = create_animated_sprite(new_sprite, 30, 5,
                                                  (xpm_map_t) mob_idle_2,
                                                  (xpm_map_t) mob_idle_3,
                                                  (xpm_map_t) mob_idle_2,
                                                  (xpm_map_t) mob_idle_1);
    mob->max_speed = MOB_MAX_SPEED;
    mob->state = MOB_IDLE;
    return mob;
}

void destroy_mob(Mob *mob) {
    if (!mob)
        return;
    destroy_animated_sprite(mob->animated_sprite);
    free(mob);
}

/* Getter and setter section */

Sprite *mob_get_sprite(Mob *mob) {
    if (!mob)
        return NULL;

    return mob->animated_sprite->sprite;
}

unsigned char mob_get_health(Mob *mob) {
    if (!mob)
        return 0;
    return mob->health;
}

void mob_set_health(Mob *mob, unsigned char health) {
    if (!mob)
        return;
    
    mob->health = health;
}

int mob_get_max_speed(Mob *mob) {
    if (!mob)
        return 0;

    return mob->max_speed;
}


MobState mob_get_state(Mob *mob) {
    if (!mob)
        return MOB_DEAD;
    return mob->state;
}

/* Statics section */

/* Others section */

void mob_update_state(Mob *mob, uint16_t player_cx, uint16_t player_cy) {
    if (mob == NULL)
        return;

    if (mob->health == 0) {
        mob->state = MOB_DEAD;
        return;
    }

    Sprite *sprite = mob->animated_sprite->sprite;
    uint16_t mob_x = sprite->x;
    uint16_t mob_y = sprite->y;

    uint16_t mob_cx = mob_x + sprite->width / 2;
    uint16_t mob_cy = mob_y + sprite->height / 2;

    int dx = player_cx - mob_cx;
    int dy = player_cy - mob_cy;
    if (abs(dx) < MOB_RADIUS && abs(dy) < MOB_RADIUS) {
        bool new_direction = dx > 0;
        if (mob->state != MOB_ATTACKING || mob->direction != new_direction) {
            Sprite *new_sprite = create_sprite(new_direction ? (xpm_map_t) mob_attacking_right : (xpm_map_t) mob_attacking_left, mob_x, mob_y, 0, 0);
            mob->animated_sprite = create_animated_sprite(new_sprite, 30, 1);
        }
        sprite->xspeed = dx == 0 ? 0 : dx / abs(dx);
        sprite->yspeed = dy == 0 ? 0 : dy / abs(dy);
        mob->state = MOB_ATTACKING;
        mob->direction = new_direction;
    } else {
        if (mob->state != MOB_IDLE) {
            Sprite *new_sprite = create_sprite((xpm_map_t) mob_idle_1, mob_x, mob_y, 0, 0);
            mob->animated_sprite = create_animated_sprite(new_sprite, 30, 5,
                                                          (xpm_map_t) mob_idle_2,
                                                          (xpm_map_t) mob_idle_3,
                                                          (xpm_map_t) mob_idle_2,
                                                          (xpm_map_t) mob_idle_1);
        }
        mob->state = MOB_IDLE;
    }
}

void mob_limit_speed(Mob *mob) {
    if (mob->animated_sprite->sprite->xspeed >= mob->max_speed) {
        mob->animated_sprite->sprite->xspeed = mob->max_speed;
    }
    if (mob->animated_sprite->sprite->xspeed >= mob->max_speed) {
        mob->animated_sprite->sprite->xspeed = mob->max_speed;
    }
}

void mob_stop(Mob *mob) {
    mob->animated_sprite->sprite->xspeed = MOB_DEFAULT_SPEED;
    mob->animated_sprite->sprite->xspeed = MOB_DEFAULT_SPEED;
}

/* Draw section */

void draw_mob(Mob *mob, uint8_t *frame_buffer) {
    draw_animated_sprite(mob->animated_sprite, frame_buffer);
}
