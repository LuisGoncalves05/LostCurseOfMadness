#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20

#define HEALTH 3

#define PLAYER_MAX_SPEED 7
#define PLAYER_ACCELERATION 2
#define PLAYER_HEALTH 3
#define PLAYER_DEFAULT_SPEED 5

// Player structure
typedef struct Player Player;


// Player functions
Player *create_player(Sprite *sprite);
void destroy_player(Player *player);
int draw_player(Player *player);
void keyboard_handler(Player *player);
void mouse_handler(Player *player, struct packet pp);


#endif
