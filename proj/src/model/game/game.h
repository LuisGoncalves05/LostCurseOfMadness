#include "lcom/lcf.h"
#include "Player.h"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20

#define HEALTH 3

typedef enum {
    MENU,
    LEVEL,
    VICTORY,
    GAME_OVER,
    EXIT
} State;

void main_game_loop();

int draw_player();

void keyboard_handler();
void mouse_handler(struct packet pp);
