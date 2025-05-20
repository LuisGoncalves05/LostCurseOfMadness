#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/video/gpu.h"


int draw_text(char *text, uint16_t x, uint16_t y, uint32_t color);

int copy_frame_buffer();

int(clear)(uint8_t *frame_buffer);

#endif
