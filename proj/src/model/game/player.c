#include "player.h"
#include "lcom/lcf.h"
#include "../Sprite.h"
#include "../../view/view.h"
#include <stdlib.h>

extern uint8_t scan_code;

struct Player {
    int health;         // Player health
    int max_speed;
    int acceleration;
    Sprite *sprite;     // Player animated sprite
    int moved;       // Player moved flag
};

/* static */ void playerIsAtMaxSpeed(Player *player){
    if(player->sprite->xspeed >= player->max_speed){
        player->sprite->xspeed = player->max_speed;
    }
    if(player->sprite->yspeed >= player->max_speed){
        player->sprite->yspeed = player->max_speed;
    }
}

/* static */ void playerStopped(Player *player){
    if(player->moved == 0){
        player->sprite->xspeed = PLAYER_DEFAULT_SPEED;
        player->sprite->yspeed = PLAYER_DEFAULT_SPEED;
    }
}

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

int draw_player(Player *player) {
    draw_sprite_xpm(player->sprite, player->sprite->x, player->sprite->y);
    
    return 0;
}

void keyboard_handler(Player *player){
    bool moved =0;
    if(scan_code == KEY_W){
        player->sprite->y -= player->sprite->yspeed;
        moved = 1;
        
    }
    else if(scan_code == KEY_A){
        player->sprite->x -=  player->sprite->xspeed;
        moved = 1;
    }
    else if(scan_code == KEY_S){
        player->sprite->y +=  player->sprite->yspeed;
        moved = 1;
    }
    else if(scan_code == KEY_D){
        player->sprite->x +=  player->sprite->xspeed;
        moved = 1;
    }
    player->moved = moved;
    
}

void mouse_handler(Player *player, struct packet pp){

}
