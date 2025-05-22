#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define FOV_RADIUS 600

extern uint8_t *maze_buffer;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res;
extern uint16_t y_res;
extern uint32_t frame_size;

extern uint8_t scan_code;
extern struct packet pp;

typedef struct Game Game;

typedef enum {
  MENU,
  LEVEL,
  VICTORY,
  GAME_OVER,
  EXIT
} State;

Game *create_game();
void destroy_game(Game *game);

State get_state(Game *game);

void game_timer_handler(Game *game);
void game_keyboard_handler(Game *game);
void game_mouse_handler(Game *game);

#endif
