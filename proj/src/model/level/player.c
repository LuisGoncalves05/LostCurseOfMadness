#include "player.h"

struct Player {
    AnimatedSprite *animated_sprite;
    unsigned char health;
    PlayerState state;
};

/* Create and destroy section */

Player *create_player() {
    Player *player = (Player *) malloc(sizeof(Player));
    if (!player)
        return NULL;
    player->state = PLAYER_IDLE;
    player->health = PLAYER_HEALTH;
    Sprite *new_sprite = create_sprite((xpm_map_t) player_idleS_1, 0, 0, 0, 0);
    player->animated_sprite = create_animated_sprite(new_sprite, 45, 2, (xpm_map_t) player_idleS_2);
    return player;
}

void destroy_player(Player *player) {
    if (!player) return;
    destroy_animated_sprite(player->animated_sprite);
    free(player);
}

AnimatedSprite *player_get_animated_sprite(Player *player) {
    if (player == NULL)
        return NULL;
    return player->animated_sprite;
}

Sprite *player_get_sprite(Player *player) {
    if (player == NULL)
        return NULL;
    return player->animated_sprite->sprite;
}

void player_set_sprite(Player *player, AnimatedSprite *sprite) {
    if (player == NULL) return;
    if (player->animated_sprite != NULL) {
        destroy_animated_sprite(player->animated_sprite);
    }
    player->animated_sprite = sprite;
}

unsigned char player_get_health(Player *player) {
    return player->health;
}

void player_set_health(Player *player, unsigned char health) {
    player->health = health;
}

PlayerState player_get_state(Player *player) {
    return player->state;
}

/* Statics section */

/* Others section */

void player_update_state(Player *player, struct packet pp) {
    if (player == NULL)
        return;

    if (player->animated_sprite->sprite->xspeed != 0 || player->animated_sprite->sprite->yspeed != 0) {
        player->state = PLAYER_WALKING;
    } else {
        player->state = PLAYER_IDLE;
    }

    if (pp.lb == 1) {
        player->state = PLAYER_SHOOTING;
    }

    if (player->health == 0) {
        player->state = PLAYER_DYING;
    }
}

void player_update_speed(Player *player, uint8_t scan_code) {
  switch (scan_code) {
    case KEY_W:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleW_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                player->animated_sprite->sprite->xspeed,
                -PLAYER_DEFAULT_SPEED),
            45, 2, (xpm_map_t) player_idleW_2);
        break;
    case KEY_A:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleA_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                -PLAYER_DEFAULT_SPEED,
                player->animated_sprite->sprite->yspeed),
            45, 2, (xpm_map_t) player_idleA_2);
        break;
    case KEY_S:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleS_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                player->animated_sprite->sprite->xspeed,
                PLAYER_DEFAULT_SPEED),
            45, 2, (xpm_map_t) player_idleS_2);
        break;
    case KEY_D:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleD_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                PLAYER_DEFAULT_SPEED,
                player->animated_sprite->sprite->yspeed),
            45, 2, (xpm_map_t) player_idleD_2);
        break;
    case KEY_BREAK_W:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleW_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                player->animated_sprite->sprite->xspeed,
                0),
            45, 2, (xpm_map_t) player_idleW_2);
        break;
    case KEY_BREAK_A:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleA_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                0,
                player->animated_sprite->sprite->yspeed),
            45, 2, (xpm_map_t) player_idleA_2);
        break;
    case KEY_BREAK_S:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleS_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                player->animated_sprite->sprite->xspeed,
                0),
            45, 2, (xpm_map_t) player_idleS_2);
        break;
    case KEY_BREAK_D:
        player->animated_sprite = create_animated_sprite(
            create_sprite(
                (xpm_map_t) player_idleD_1,
                player->animated_sprite->sprite->x,
                player->animated_sprite->sprite->y,
                0,
                player->animated_sprite->sprite->yspeed),
            45, 2, (xpm_map_t) player_idleD_2);
        break;
    default:
        break;
  }
}

/* Draw section */

void draw_player(Player *player, double delta, uint8_t *frame_buffer) {
    draw_animated_sprite(player->animated_sprite, frame_buffer);
}
