#include "Player.h"
#include "lcom/lcf.h"
#include "../Sprite.h"
#include "../../view/view.h"
#include <stdlib.h>

// Create a new player
Player *create_player(Sprite *sprite) {
    Player *player = (Player *)malloc(sizeof(Player));
    if (!player) return NULL;
    player->health = PLAYER_HEALTH;
    player->sprite = sprite;
    player->moved = 0;
    player->max_speed = PLAYER_MAX_SPEED; // Set default max speed
    player->acceleration = PLAYER_ACCELERATION; // Set default acceleration
    return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
    if (!player) return;

    if (player->sprite) {
        destroy_sprite(player->sprite);
    }
    free(player);
}

void playerIsAtMaxSpeed(Player *player){
    if(player->sprite->xspeed >= player->max_speed){
        player->sprite->xspeed = player->max_speed;
    }
    if(player->sprite->yspeed >= player->max_speed){
        player->sprite->yspeed = player->max_speed;
    }
}

void playerStopped(Player *player){
    if(player->moved == 0){
        player->sprite->xspeed = PLAYER_DEFAULT_SPEED;
        player->sprite->yspeed = PLAYER_DEFAULT_SPEED;
    }
}

int draw_player(Player **player, Sprite **player_sprite) {

    if(*player == NULL){
        *player_sprite = create_sprite((xpm_map_t) cross, 10, 10, 3, 3);
        *player = create_player(*player_sprite);
        
    }
    draw_sprite_xpm((*player)->sprite, (*player)->sprite->x, (*player)->sprite->y);
    
    return 0;
}

