#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#define BUTTON_PLAY_START_X 212
#define BUTTON_PLAY_START_Y 400
#define BUTTON_EXIT_START_X 522
#define GAME_OVER_BUTTON_START_Y 400

#define BUTTON_WIDTH 290
#define BUTTON_HEIGHT 139

#include <lcom/lcf.h>
#include <stdint.h>
#include "button.h"
typedef struct MainMenu MainMenu;

MainMenu *create_main_menu();
void destroy_main_menu(MainMenu *main_menu);

void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer);
void main_menu_set_button(MainMenu *main_menu, Button button);
Button main_menu_get_button(MainMenu *main_menu);
Button main_menu_click_handler(MainMenu *main_menu, double x_mouse, double y_mouse);

#endif
