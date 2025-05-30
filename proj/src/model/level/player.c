#include "player.h"
#include "maze.h"

struct Player {
    AnimatedSprite *animated_sprite;
    unsigned char health;
    Direction direction;
    PlayerState state;
};

/* Create and destroy section */

Player *create_player() {
    Player *player = (Player *) malloc(sizeof(Player));
    if (player == NULL) {
        printf("create_player: NULL pointer provided\n");
        return NULL;
    }

    player->state = PLAYER_IDLE;
    player->health = PLAYER_HEALTH;
    Sprite *new_sprite = create_sprite((xpm_map_t) player_idleS_1, CELL_SIZE, CELL_SIZE, 0, 0);
    if (new_sprite == NULL) {
        printf("create_player: create_sprite failed\n");
        return NULL;
    }

    new_sprite->x += (CELL_SIZE - new_sprite->width) / 2;
    new_sprite->y += (CELL_SIZE - new_sprite->height) / 2;
    player->animated_sprite = create_animated_sprite(new_sprite, 45, 2, (xpm_map_t) player_idleS_2);
    if (player->animated_sprite == NULL) {
        printf("create_player: create_sprite failed\n");
        free(new_sprite);
        return NULL;
    }
    
    player->direction = DOWN;
    return player;
}

void destroy_player(Player *player) {
    if (player == NULL) {
        printf("destroy_player: NULL pointer provided\n");
        return;
    }
    destroy_animated_sprite(player->animated_sprite);
    free(player);
}

AnimatedSprite *player_get_animated_sprite(Player *player) {
    if (player == NULL) {
        printf("player_get_animated_sprite: NULL pointer provided\n");
        return NULL;
    }

    return player->animated_sprite;
}

Sprite *player_get_sprite(Player *player) {
    if (player == NULL) {
        printf("player_get_sprite: NULL pointer provided\n");
        return NULL;
    }

    return player->animated_sprite->sprite;
}

uint16_t player_get_x(Player *player) {
    if (player == NULL) {
        printf("player_get_x: NULL pointer provided\n");
        return 0;
    }

    return player->animated_sprite->sprite->x;
}

void player_set_x(Player *player, uint16_t x) {
    if (player == NULL) {
        printf("player_set_x: NULL pointer provided\n");
        return;
    }
    player->animated_sprite->sprite->x = x;
}

uint16_t player_get_y(Player *player) {
    if (player == NULL) {
        printf("player_get_y: NULL pointer provided\n");
        return 0;
    }

    return player->animated_sprite->sprite->y;
}

void player_set_y(Player *player, uint16_t y) {
    if (player == NULL) {
        printf("player_set_y: NULL pointer provided\n");
        return;
    }
    player->animated_sprite->sprite->y = y;
}

double player_get_xspeed(Player *player) {
    if (player == NULL) {
        printf("player_get_xspeed: NULL pointer provided\n");
        return 0;
    }
    return player->animated_sprite->sprite->xspeed;
}

void player_set_xspeed(Player *player, double xspeed) {
    if (player == NULL) {
        printf("player_set_xspeed: NULL pointer provided\n");
        return;
    }
    player->animated_sprite->sprite->xspeed = xspeed;
}

double player_get_yspeed(Player *player) {
    if (player == NULL) {
        printf("player_get_yspeed: NULL pointer provided\n");
        return 0;
    }
    return player->animated_sprite->sprite->yspeed;
}

void player_set_yspeed(Player *player, double yspeed) {
    if (player == NULL) {
        printf("player_set_yspeed: NULL pointer provided\n");
        return;
    }
    player->animated_sprite->sprite->yspeed = yspeed;
}

uint16_t player_get_width(Player *player) {
    if (player == NULL) {
        printf("player_get_width: NULL pointer provided\n");
        return 0;
    }

    return player->animated_sprite->sprite->width;
}

uint16_t player_get_height(Player *player) {
    if (player == NULL) {
        printf("player_get_height: NULL pointer provided\n");
        return 0;
    }

    return player->animated_sprite->sprite->height;
}

Direction player_get_direction(Player *player) {
    if (player == NULL) {
        printf("player_get_direction: NULL pointer provided\n");
        return DOWN;
    }

    return player->direction;
}

void player_set_sprite(Player *player, AnimatedSprite *sprite) {
    if (player == NULL || sprite == NULL) {
        printf("player_set_sprite: NULL pointer provided\n");
        return;
    }
    
    if (destroy_animated_sprite(player->animated_sprite)) {
        return;
    }
    player->animated_sprite = sprite;
}

PlayerState player_get_state(Player *player) {
    if (player == NULL) {
        printf("player_get_state: NULL pointer provided\n");
        return PLAYER_DEAD;
    }

    return player->state;
}

void player_set_state(Player *player, PlayerState state) {
    if (player == NULL) {
        printf("player_set_state: NULL pointer provided\n");
        return;
    }

    player->state = state;
}

/* Statics section */

/* Others section */

void player_update_state(Player *player, struct packet pp) {
    if (player == NULL) {
        printf("player_update_state: NULL pointer provided\n");
        return;
    }

    if (player->state == PLAYER_WIN) {
        return;
    }

    if (player->animated_sprite->sprite->xspeed == 0 && player->animated_sprite->sprite->yspeed == 0) {
        player->state = PLAYER_IDLE;
    }

    if (pp.lb == 1) {
        player->state = PLAYER_SHOOTING;
    }

    if (player->health == 0) {
        player->state = PLAYER_DEAD;
    }
}

void player_update_speed(Player *player, uint8_t scan_code) {
    if (player == NULL)
        return;
    
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
