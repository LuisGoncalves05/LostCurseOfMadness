#include "Player.h"
#include <stdlib.h>

// Create a new player
Player *create_player(int health, int speed_x, int speed_y, AnimSprite *sprite) {
    Player *player = (Player *)malloc(sizeof(Player));
    if (!player) return NULL;
    player->health = health;
    player->speed_x = speed_x;
    player->speed_y = speed_y;
    player->sprite = sprite;

    return player;
}

// Destroy the player and free memory
void destroy_player(Player *player) {
    if (!player) return;

    if (player->sprite) {
        destroy_anim_sprite(player->sprite);
    }
    free(player);
}