#ifndef BUTTON_H
#define BUTTON_H

#define BUTTON_MENU_START_X 212
#define BUTTON_EXIT_START_X 522
#define MAIN_MENU_BUTTON_START_Y 500
#define GAME_OVER_BUTTON_START_Y 400

#define BUTTON_WIDTH 290
#define BUTTON_HEIGHT 139

#include "controller/video/gpu.h"
#include "assets/xpm/game_over.xpm"
#include "assets/xpm/lcom.xpm"
#include "assets/xpm/play_button.xpm"
#include "assets/xpm/menu_button.xpm"
#include "assets/xpm/exit_button.xpm"

typedef enum {
  BUTTON_NONE,
  BUTTON_PLAY,
  BUTTON_MENU,
  BUTTON_EXIT
} Button;

void draw_button(Button button, bool selected, uint16_t x, uint16_t y, uint8_t *frame_buffer);

#endif
