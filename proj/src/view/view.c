#include "view.h"

uint8_t *sec_frame_buffer;
extern vbe_mode_info_t vg_mode_info;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res, y_res;
extern uint8_t *main_frame_buffer;
uint32_t frame_size;


int draw_sprite_xpm(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->map[w + h*width];
        if (vg_draw_pixel(x + w, y + h, current_color, sec_frame_buffer) != 0) return 1;
      }
    }
    return 0; 
}

int set_frame_buffers(uint16_t mode){
  if(set_frame_buffer(mode)!= 0){
    printf("Error setting frame buffer\n");
    return 1;
  }
  frame_size = x_res * y_res * bytes_per_pixel;
  sec_frame_buffer = (uint8_t *) (uint8_t *) malloc(frame_size);
  if (sec_frame_buffer == NULL) {
    printf("Error allocating memory for secondary frame buffer\n");
    return 1;
  }
  return 0;
}

int copy_frame_buffer(){
  memcpy(main_frame_buffer, sec_frame_buffer, frame_size);
  if(main_frame_buffer == NULL){
    printf("Error copying frame buffer\n");
    return 1;
  }
  return 0;
}



