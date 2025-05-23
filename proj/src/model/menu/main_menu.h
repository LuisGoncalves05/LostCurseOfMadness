#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "button.h"

#include "assets/xpm/lcom.xpm"
#include "assets/xpm/play_button.xpm"
#include "assets/xpm/play_button_selected.xpm"

typedef struct MainMenu MainMenu;

MainMenu *create_main_menu();
void destroy_main_menu(MainMenu *main_menu);

void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer);
void main_menu_set_button(MainMenu *main_menu, Button button);
void main_menu_change_button(MainMenu *main_menu);
ButtonType main_menu_click_handler(MainMenu *main_menu, uint16_t x, uint16_t y);

#endif
