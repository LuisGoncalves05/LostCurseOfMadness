#include "player.h"
#include "maze.h"

struct Player {
    AnimatedSprite *animated_sprite;
    unsigned char health;
    PlayerState state;
    Direction direction;
};

/* Create and destroy section */

Player *create_player() {
    Player *player = (Player *) malloc(sizeof(Player));
    if (!player)
        return NULL;
    player->state = PLAYER_IDLE;
    player->health = PLAYER_HEALTH;
    Sprite *new_sprite = create_sprite((xpm_map_t) player_idleS_1, CELL_SIZE, CELL_SIZE, 0, 0);
    new_sprite->x += (CELL_SIZE - new_sprite->width) / 2;
    new_sprite->y += (CELL_SIZE - new_sprite->height) / 2;
    player->animated_sprite = create_animated_sprite(new_sprite, 45, 2, (xpm_map_t) player_idleS_2);
    player->direction = DOWN;
    return player;
}

void destroy_player(Player *player) {
    if (!player)
        return;
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

uint16_t player_get_x(Player *player) {
    if (player == NULL)
        return 0;
    return player->animated_sprite->sprite->x;
}

uint16_t player_get_y(Player *player) {
    if (player == NULL)
        return 0;
    return player->animated_sprite->sprite->y;
}

uint16_t player_get_width(Player *player) {
    if (player == NULL)
        return 0;
    return player->animated_sprite->sprite->width;
}

uint16_t player_get_heigth(Player *player) {
    if (player == NULL)
        return 0;
    return player->animated_sprite->sprite->height;
}

PlayerState player_get_state(Player *player) {
    return player->state;
}

void player_set_state(Player *player, PlayerState state) {
    player->state = state;
}

/* Statics section */

/* Others section */

void player_update_state(Player *player, struct packet pp) {
    if (player == NULL || player->state == PLAYER_WIN)
        return;

    if (player->animated_sprite->sprite->xspeed != 0 || player->animated_sprite->sprite->yspeed != 0) {
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
            if (player->state != PLAYER_WALKING || player->direction != UP) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_walkW_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    player->animated_sprite->sprite->xspeed,
                    -PLAYER_DEFAULT_SPEED),
                10, 4, (xpm_map_t) player_walkW_2, (xpm_map_t) player_walkW_3, (xpm_map_t) player_walkW_2);
            player->state = PLAYER_WALKING;
            player->direction = UP;
            break;
        case KEY_A:
            if (player->state != PLAYER_WALKING || player->direction != LEFT) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_walkA_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    -PLAYER_DEFAULT_SPEED,
                    player->animated_sprite->sprite->yspeed),
                10, 4, (xpm_map_t) player_walkA_2, (xpm_map_t) player_walkA_3, (xpm_map_t) player_walkA_2);
            player->state = PLAYER_WALKING;
            player->direction = LEFT;
            break;
        case KEY_S:
            if (player->state != PLAYER_WALKING || player->direction != DOWN) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_walkS_3,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    player->animated_sprite->sprite->xspeed,
                    PLAYER_DEFAULT_SPEED),
                10, 4, (xpm_map_t) player_walkS_2, (xpm_map_t) player_walkS_1, (xpm_map_t) player_walkS_2);
            player->state = PLAYER_WALKING;
            player->direction = DOWN;
            break;
        case KEY_D:
            if (player->state != PLAYER_WALKING || player->direction != RIGHT) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_walkD_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    PLAYER_DEFAULT_SPEED,
                    player->animated_sprite->sprite->yspeed),
                10, 4, (xpm_map_t) player_walkD_2, (xpm_map_t) player_walkD_3, (xpm_map_t) player_walkD_2);
            player->state = PLAYER_WALKING;
            player->direction = RIGHT;
            break;
        case KEY_BREAK_W:
            if (player->state != PLAYER_WALKING || player->direction == UP) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_walkW_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    player->animated_sprite->sprite->xspeed,
                    0),
                45, 2, (xpm_map_t) player_idleW_2);
            player->state = PLAYER_IDLE;
            break;
        case KEY_BREAK_A:
            if (player->state != PLAYER_WALKING || player->direction == LEFT) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_idleA_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    0,
                    player->animated_sprite->sprite->yspeed),
                45, 2, (xpm_map_t) player_idleA_2);
            player->state = PLAYER_IDLE;
            break;
        case KEY_BREAK_S:
            if (player->state != PLAYER_WALKING || player->direction == DOWN) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_idleS_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    player->animated_sprite->sprite->xspeed,
                    0),
                45, 2, (xpm_map_t) player_idleS_2);
            player->state = PLAYER_IDLE;
            break;
        case KEY_BREAK_D:
            if (player->state != PLAYER_WALKING || player->direction == RIGHT) player->animated_sprite = create_animated_sprite(
                create_sprite(
                    (xpm_map_t) player_idleD_1,
                    player->animated_sprite->sprite->x,
                    player->animated_sprite->sprite->y,
                    0,
                    player->animated_sprite->sprite->yspeed),
                45, 2, (xpm_map_t) player_idleD_2);
            player->state = PLAYER_IDLE;
            break;
        default:
            break;
    }
}

void player_lose_health(Player *player) {
    if (player == NULL)
        return;
    if (player->health > 0)
        player->health--;
}

/* Draw section */

void draw_player(Player *player, double delta, uint8_t *frame_buffer) {
    draw_animated_sprite(player->animated_sprite, frame_buffer);
}
