#ifndef GAME_OVER_H
#define GAME_OVER_H


#include <lcom/lcf.h>
#include <stdint.h>
#include "assets/xpm/game_over_indexed.xpm"
#include "controller/video/gpu.h"


typedef struct GameOver GameOver;

GameOver *create_game_over();
void destroy_game_over(GameOver *game_over);

void draw_game_over(GameOver *game_over, uint8_t *frame_buffer);

#endif
