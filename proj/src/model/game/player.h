#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"
#include "game.h"
#include "../../controller/video/gpu.h"
#include "../../images/font/cursor.xpm"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20
#define KEY_X 0x2D
#define HEALTH 3

#define PLAYER_MAX_SPEED 7
#define PLAYER_ACCELERATION 2
#define PLAYER_HEALTH 3
#define PLAYER_DEFAULT_SPEED 5

xpm_image_t	img;
extern double	delta;
extern double	direction;

extern double	x_mouse;
extern double	y_mouse;

typedef struct Player Player;

struct Player {
	unsigned char health;
	int max_speed;
	int acceleration;
	Sprite *sprite;
	bool moved;
};

// Player functions
Player *create_player(Sprite *sprite);
void destroy_player(Player *player);
int draw_player(Player *player);
void keyboard_handler(Player *player);
void mouse_handler(Player *player, struct packet pp);
void game_draw_cursor();
#endif
