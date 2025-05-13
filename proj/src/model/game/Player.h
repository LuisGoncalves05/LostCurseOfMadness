#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"

// Player structure
typedef struct {
    int health;         // Player health
    int max_speed;
    int acceleration;
    Sprite *sprite;     // Player animated sprite
    int moved;       // Player moved flag
} Player;

#define PLAYER_MAX_SPEED 7
#define PLAYER_ACCELERATION 2
#define PLAYER_HEALTH 3
#define PLAYER_DEFAULT_SPEED 5

// Player functions
Player *create_player(Sprite *sprite);
void destroy_player(Player *player);
void playerIsAtMaxSpeed(Player *player);
void playerStopped(Player *player);
int draw_player(Player **player, Sprite **player_sprite);

#endif
