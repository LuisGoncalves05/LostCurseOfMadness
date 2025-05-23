#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "drivers/video/gpu.h"
#include "model/sprite.h"

typedef struct Cursor Cursor;

Cursor* create_cursor(xpm_map_t xpm);
void destroy_cursor(Cursor* cursor);

uint16_t cursor_get_x(Cursor* cursor);
uint16_t cursor_get_y(Cursor* cursor);

void cursor_update(Cursor* cursor, double dx, double dy);

void draw_cursor(Cursor* cursor, uint8_t* buffer);

#endif
