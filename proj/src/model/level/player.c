#include "player.h"

struct Player {
    PlayerState state;
    AnimatedSprite *animated_sprite;
    unsigned char health;
};

// Create a new player
Player *create_player() {
    Player *player = (Player *) malloc(sizeof(Player));
    if (!player) return NULL;
    player->state = PLAYER_IDLE; 
    player->health = PLAYER_HEALTH;
    Sprite *new_sprite = create_sprite((xpm_map_t) player_idle1, 400, 400, 0, 0);
    player->animated_sprite = create_animated_sprite(new_sprite, 30, 2, (xpm_map_t) player_idle2);
    return player;
}

// Destroy the player and free memory
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

PlayerState get_player_state(Player *player) {
    return player->state;
}

void set_player_state(Player *player, PlayerState state) {
    if (player == NULL) return;
    player->state = state;
}

void player_set_health(Player *player, unsigned char health) {
    player->health = health;
}

void update_player_state(Player *player, struct packet pp) {
    if (player == NULL) return;
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
        player->animated_sprite->sprite->yspeed = -PLAYER_DEFAULT_SPEED; 
        break;
    case KEY_A:
        player->animated_sprite->sprite->xspeed = -PLAYER_DEFAULT_SPEED;
        break;
    case KEY_S:
        player->animated_sprite->sprite->yspeed = PLAYER_DEFAULT_SPEED;
        break;
    case KEY_D:
        player->animated_sprite->sprite->xspeed = PLAYER_DEFAULT_SPEED;
        break;
    case KEY_BREAK_W:
        player->animated_sprite->sprite->yspeed = 0; 
        break;
    case KEY_BREAK_A:
        player->animated_sprite->sprite->xspeed = 0;
        break;
    case KEY_BREAK_S:
        player->animated_sprite->sprite->yspeed = 0;
        break;
    case KEY_BREAK_D:
        player->animated_sprite->sprite->xspeed = 0;
        break;
    default:
        break;
  }
}

void draw_player(Player *player, double delta, uint8_t *frame_buffer) {
    draw_animated_sprite(player->animated_sprite, frame_buffer);
}
