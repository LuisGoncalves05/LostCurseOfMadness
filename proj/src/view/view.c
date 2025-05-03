#include "view.h"

int draw_sprite_xpm(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->map[w + h*width];
        if (vg_draw_pixel(x + w, y + h, current_color) != 0) return 1;
      }
    }
    return 0; 
}

