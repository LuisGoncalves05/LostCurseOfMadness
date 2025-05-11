#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimSprite.h"

// Player structure
typedef struct {
    int health;         // Player health
    AnimSprite *sprite;     // Player animated sprite
} Player;

// Player functions
Player *create_player(int health, AnimSprite *sprite);
void destroy_player(Player *player);

#endif