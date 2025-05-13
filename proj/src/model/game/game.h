#include "lcom/lcf.h"
#include "Player.h"

#define KEY_W 0x11
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20


typedef enum {
    Waiting,
    Running,
    Finish,
} GameState;


void main_game_loop();

void keyboard_handler();
void mouse_handler(struct packet pp);
