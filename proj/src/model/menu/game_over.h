#ifndef GAME_OVER_H
#define GAME_OVER_H

#define BUTTON_MENU_START_X 212
#define BUTTON_MENU_START_Y 400
#define BUTTON_EXIT_START_X 522
#define BUTTON_EXIT_START_Y 400

#define BUTTON_WIDTH 290
#define BUTTON_HEIGHT 139

#include <lcom/lcf.h>
#include <stdint.h>
#include "drivers/video/gpu.h"
#include "assets/xpm/game_over_indexed.xpm"
#include "assets/xpm/menu_button.xpm"
#include "assets/xpm/exit_button.xpm"

typedef enum {
    BUTTON_MENU,
    BUTTON_EXIT,
    BUTTON_NONE
} GameOverButton;

typedef struct GameOver GameOver;

GameOver *create_game_over();
void destroy_game_over(GameOver *game_over);

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer);
void game_over_set_button(GameOver *game_over, GameOverButton button);
GameOverButton game_over_get_button(GameOver *game_over);
GameOverButton game_over_click_handler(GameOver *game_over, double x_mouse, double y_mouse);

#endif
