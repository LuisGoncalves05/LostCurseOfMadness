#ifndef PLAYER_H
#define PLAYER_H

#include "../AnimSprite.h"

// Player structure
typedef struct {
    int health;         // Player health
    int speed_x, speed_y;   // Player speed
    AnimSprite *sprite;     // Player animated sprite
} Player;

// Player functions
Player *create_player(int health, int speed_x, int speed_y, AnimSprite *sprite);
void destroy_player(Player *player);

#endif