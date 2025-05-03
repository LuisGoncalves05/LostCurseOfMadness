#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "config.h"
#include "controller/video/gpu.h"
#include "model/Sprite.h"


int draw_sprite_xpm(Sprite *sprite, int x, int y);

#endif