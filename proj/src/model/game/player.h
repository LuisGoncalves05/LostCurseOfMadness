#ifndef PLAYER_H
#define PLAYER_H

#include "../Sprite.h"
#include "game.h"
#include "../../controller/video/gpu.h"
#include "img/cursor.xpm"

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

#define FOV_RADIUS 600
//#define FOV_ANGLE 75.0      // Ângulo do cone de visão em graus
extern double fov_angle;

xpm_image_t	img;
extern double	delta;
extern double	direction;

extern double	x_mouse;
extern double	y_mouse;


enum player_state {
	PLAYER_IDLE,
	PLAYER_WALKING,
	PLAYER_AIMING,
	PLAYER_SHOOTING,
	PLAYER_DYING,
};

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
void keyboard_handler(Player *player, Maze *maze);
void mouse_handler(Player *player, struct packet pp);
void game_draw_cursor();
void game_draw_fov_cone(Player *player, Maze* maze);
void init_maze_buffer();
void free_maze_buffer();


void update_player_state(Player *player, struct packet pp);
void cursor_check_bound();
#endif

