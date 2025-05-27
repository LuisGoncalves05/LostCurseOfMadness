#include "mob.h"

struct Mob {
    unsigned char health;
    int max_speed;
    int acceleration;
    AnimatedSprite *animated_sprite;
    bool moved;
    enum mob_state state;
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
        (xpm_map_t) mob_idle_1
    );
    mob->moved = 0;
    mob->max_speed = MOB_MAX_SPEED;
    mob->acceleration = MOB_ACCELERATION;
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
    return mob->animated_sprite->sprite;
}

unsigned char mob_get_health(Mob *mob) {
    return mob->health;
}

void mob_set_health(Mob *mob, unsigned char health) {
    mob->health = health;
}

int mob_get_max_speed(Mob *mob) {
    return mob->max_speed;
}

int mob_get_acceleration(Mob *mob) {
    return mob->acceleration;
}

bool mob_get_moved(Mob *mob) {
    return mob->moved;
}

void mob_set_moved(Mob *mob, bool moved) {
    mob->moved = moved;
}

enum mob_state mob_get_state(Mob *mob) {
    return mob->state;
}

/* Statics section */

/* Others section */

void update_mob_state(Mob *mob) {
    if (mob == NULL)
        return;
    
    if (mob->moved == 1) {
        mob->state = MOB_WALKING;
    } else {
        mob->state = MOB_IDLE;
    }
}

void mobIsAtMaxSpeed(Mob *mob) {
    if (mob->animated_sprite->sprite->xspeed >= mob->max_speed) {
        mob->animated_sprite->sprite->xspeed = mob->max_speed;
    }
    if (mob->animated_sprite->sprite->xspeed >= mob->max_speed) {
        mob->animated_sprite->sprite->xspeed = mob->max_speed;
    }
}

void mobStopped(Mob *mob) {
    if (mob->moved == 0) {
        mob->animated_sprite->sprite->xspeed = MOB_DEFAULT_SPEED;
        mob->animated_sprite->sprite->xspeed = MOB_DEFAULT_SPEED;
    }
}

/* Draw section */

void draw_mob(Mob *mob, uint8_t *frame_buffer) {
    draw_animated_sprite(mob->animated_sprite, frame_buffer);
}
