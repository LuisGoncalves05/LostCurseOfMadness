#include "lcom/lcf.h"
#include "Player.h"
#include "player.xpm"

typedef enum {
    Waiting,
    Running,
    Finish,
} GameState;


void main_game_loop();

int draw_player();

