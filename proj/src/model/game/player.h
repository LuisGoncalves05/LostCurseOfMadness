#ifndef PLAYER_H
#define PLAYER_H

#include "../sprite.h"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20
#define KEY_X 0x2D
#define KEY_C 0x2E
#define KEY_SPACE 0x39
#define HEALTH 3

#define PLAYER_MAX_SPEED 7
#define PLAYER_ACCELERATION 2
#define PLAYER_HEALTH 3
#define PLAYER_DEFAULT_SPEED 5

typedef struct Player Player;

void playerIsAtMaxSpeed(Player *player);
void playerStopped(Player *player);

Player *create_player(Sprite *sprite);
void destroy_player(Player *player);

Sprite *get_sprite(Player *player);
unsigned char get_health(Player *player);
int get_max_speed(Player *player);
int get_acceleration(Player *player);
bool get_moved(Player *player);
void set_moved(Player *player, bool moved);

#endif
