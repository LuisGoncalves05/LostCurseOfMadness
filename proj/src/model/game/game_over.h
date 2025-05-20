#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "controller/video/gpu.h"
#include "assets/xpm/game_over_indexed.xpm"
#include "assets/xpm/menu_button.xpm"
#include "assets/xpm/exit_button.xpm"

typedef enum {
    BUTTON_MENU,
    BUTTON_EXIT
} Button;

typedef struct GameOver GameOver;

GameOver *create_game_over();
void destroy_game_over(GameOver *game_over);

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer);

#endif
