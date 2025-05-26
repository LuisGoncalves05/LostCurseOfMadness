#include "player.h"

struct Player {
    PlayerState state;
    Sprite *sprite;
    AnimatedSprite *animated_sprite;
    unsigned char health;
};

// Create a new player
Player *create_player() {
    Player *player = (Player *) malloc(sizeof(Player));
    if (!player) return NULL;
    player->state = PLAYER_IDLE; 
    player->health = PLAYER_HEALTH;
    player->sprite = create_sprite((xpm_map_t) cross, 400, 400, 0, 0);

    Sprite *new_sprite = create_sprite((xpm_map_t) player_idle1, player->sprite->x, player->sprite->y, player->sprite->xspeed, player->sprite->yspeed);
    player->animated_sprite = create_animated_sprite(new_sprite, 15, 2, (xpm_map_t) player_idle2);
    return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
    if (!player) return;
    destroy_sprite(player->sprite);
    free(player);
}

Sprite *player_get_sprite(Player *player) {
    return player->sprite;
}

void player_set_sprite(Player *player, Sprite *sprite) {
    if (player == NULL) return;
    if (player->sprite != NULL) {
        destroy_sprite(player->sprite);
    }
    player->sprite = sprite;
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
    if (player->sprite->xspeed != 0 || player->sprite->yspeed != 0) {
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
        player->sprite->yspeed = -PLAYER_DEFAULT_SPEED; 
        break;
    case KEY_A:
        player->sprite->xspeed = -PLAYER_DEFAULT_SPEED;
        break;
    case KEY_S:
        player->sprite->yspeed = PLAYER_DEFAULT_SPEED;
        break;
    case KEY_D:
        player->sprite->xspeed = PLAYER_DEFAULT_SPEED;
        break;
    case KEY_BREAK_W:
        player->sprite->yspeed = 0; 
        break;
    case KEY_BREAK_A:
        player->sprite->xspeed = 0;
        break;
    case KEY_BREAK_S:
        player->sprite->yspeed = 0;
        break;
    case KEY_BREAK_D:
        player->sprite->xspeed = 0;
        break;
    default:
        break;
  }
}

void draw_player(Player *player, double delta, uint8_t *frame_buffer) {
    Sprite *player_sprite = player->sprite;
    Sprite *new_sprite;

    extern int frame_counter;
    if(frame_counter > 16) frame_counter = 0;

    switch (player->state) {
        case PLAYER_IDLE:
        new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        break;
    case PLAYER_WALKING:
        if (frame_counter <= 4){
            new_sprite = create_sprite((xpm_map_t) penguin, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        } else if (frame_counter <= 8){
            new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        } else if (frame_counter <= 12){
            new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        } else {
            new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        }
        break;  
    case PLAYER_AIMING:
        new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        break;
    case PLAYER_SHOOTING:
        new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        break;
    case PLAYER_DYING:
        new_sprite = create_sprite((xpm_map_t) pic1, player_sprite->x, player_sprite->y, player_sprite->xspeed, player_sprite->yspeed);
        break;
    default:
        break;
    }
    destroy_sprite(player_sprite);
    player_set_sprite(player, new_sprite);
    if (player->state == PLAYER_IDLE) draw_animated_sprite(player->animated_sprite, frame_buffer);
    else draw_sprite_rotated(player_get_sprite(player), delta, frame_buffer);
}
