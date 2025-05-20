#include "view.h"
#include "../controller/video/gpu.h"

extern vbe_mode_info_t vg_mode_info;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res, y_res;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint32_t frame_size;
