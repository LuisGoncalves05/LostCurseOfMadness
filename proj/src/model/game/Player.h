#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"

// Player structure
typedef struct {
    int health;         // Player health
    Sprite *sprite;     // Player animated sprite
    int moved;       // Player moved flag
} Player;

// Player functions
Player *create_player(int health, Sprite *sprite);
void destroy_player(Player *player);

#endif
