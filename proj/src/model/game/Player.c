#include "Player.h"
#include <stdlib.h>

// Create a new player
Player *create_player(int health, AnimSprite *sprite) {
    Player *player = (Player *)malloc(sizeof(Player));
    if (!player) return NULL;
    player->health = health;
    if (health <= 0) {
        free(player);
        return NULL; // Invalid health
    }
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