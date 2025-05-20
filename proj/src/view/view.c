#include "view.h"
#include "../controller/video/gpu.h"

extern vbe_mode_info_t vg_mode_info;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res, y_res;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint32_t frame_size;

int copy_frame_buffer() {
  memcpy(main_frame_buffer, sec_frame_buffer, frame_size);
  if (main_frame_buffer == NULL) {
    printf("Error copying frame buffer\n");
    return 1;
  }
  return 0;
}

int(clear)(uint8_t *frame_buffer) {
  return memset(frame_buffer, 0, x_res * y_res * bytes_per_pixel) == NULL;
}
