#ifndef PLAYER_H
#define PLAYER_H

#include "../../drivers/video/gpu.h"
#include "img/cursor.xpm"
#include "../sprite.h"

#define HEALTH 3
#define PLAYER_MAX_SPEED 7
#define PLAYER_ACCELERATION 2
#define PLAYER_HEALTH 3
#define PLAYER_DEFAULT_SPEED 5

typedef struct Player Player;

enum player_state {
	PLAYER_IDLE,
	PLAYER_WALKING,
	PLAYER_AIMING,
	PLAYER_SHOOTING,
	PLAYER_DYING,
};

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
void update_player_state(Player *player, struct packet pp);
void set_sprite(Player *player, Sprite *sprite);

#endif
