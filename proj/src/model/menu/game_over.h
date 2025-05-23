#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "button.h"

#include "assets/xpm/game_over.xpm"
#include "assets/xpm/menu_button.xpm"
#include "assets/xpm/menu_button_selected.xpm"
#include "assets/xpm/exit_button.xpm"
#include "assets/xpm/exit_button_selected.xpm"

typedef struct GameOver GameOver;

GameOver *create_game_over();
void destroy_game_over(GameOver *game_over);

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer);

/*
void game_over_set_button(GameOver *game_over, Button button);
Button game_over_get_button(GameOver *game_over);
*/
void game_over_change_button(GameOver *game_over);
ButtonType game_over_click_handler(GameOver *game_over, double x_mouse, double y_mouse);

#endif
