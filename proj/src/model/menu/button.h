#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_PLAY_START_X 212
#define BUTTON_MENU_START_X 212
#define BUTTON_EXIT_START_X 522
#define MAIN_MENU_BUTTON_START_Y 500
#define GAME_OVER_BUTTON_START_Y 400

#define BUTTON_WIDTH 290
#define BUTTON_HEIGHT 139

#include <lcom/lcf.h>
#include "drivers/video/gpu.h"

typedef enum {
    BUTTON_NONE,
    BUTTON_PLAY,
    BUTTON_MENU,
    BUTTON_EXIT
} ButtonType;

typedef struct Button Button;

Button* create_button(xpm_map_t xpm, xpm_map_t xpm_selected, uint16_t x, uint16_t y);
void destroy_button(Button *button);

bool button_get_selected(Button *button);
void button_set_selected(Button *button, bool selected);

bool button_is_clicked(Button *button, uint16_t x, uint16_t y);

void draw_button(Button *button, uint8_t *frame_buffer);

#endif
