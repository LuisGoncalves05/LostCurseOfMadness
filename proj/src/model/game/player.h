#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20

#define HEALTH 3

// Player structure
typedef struct {
    int health;         // Player health
    Sprite *sprite;     // Player animated sprite
    int moved;       // Player moved flag
} Player;

// Player functions
Player *create_player(int health, Sprite *sprite);
void destroy_player(Player *player);
int draw_player();
void keyboard_handler();
void mouse_handler(struct packet pp);


#endif
