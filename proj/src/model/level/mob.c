#include "mob.h"

struct Mob {
    unsigned char health;
    int max_speed;
    int acceleration;
    Sprite *sprite;
    bool moved;
    enum mob_state state;
};

/* Create and destroy section */

Mob *create_mob(Sprite *sprite) {
    Mob *mob = (Mob *) malloc(sizeof(Mob));
    if (!mob)
        return NULL;
    mob->health = MOB_HEALTH;
    mob->sprite = sprite;
    mob->moved = 0;
    mob->max_speed = MOB_MAX_SPEED;
    mob->acceleration = MOB_ACCELERATION;
    mob->state = MOB_IDLE;
    return mob;
}

void destroy_mob(Mob *mob) {
    if (!mob)
        return;
    destroy_sprite(mob->sprite);
    free(mob);
}

/* Getter and setter section */

Sprite *mob_get_sprite(Mob *mob) {
    return mob->sprite;
}

void mob_set_sprite(Mob *mob, Sprite *sprite) {
    if (mob == NULL)
        return;
    if (mob->sprite != NULL) {
        destroy_sprite(mob->sprite);
    }
    mob->sprite = sprite;
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
    if (mob->sprite->xspeed >= mob->max_speed) {
        mob->sprite->xspeed = mob->max_speed;
    }
    if (mob->sprite->yspeed >= mob->max_speed) {
        mob->sprite->yspeed = mob->max_speed;
    }
}

void mobStopped(Mob *mob) {
    if (mob->moved == 0) {
        mob->sprite->xspeed = MOB_DEFAULT_SPEED;
        mob->sprite->yspeed = MOB_DEFAULT_SPEED;
    }
}

/* Draw section */
