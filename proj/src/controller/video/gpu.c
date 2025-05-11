#include "gpu.h"
#include "model/Sprite.h"
#include <lcom/lcf.h>
#include <machine/int86.h>

vbe_mode_info_t vg_mode_info;
uint8_t bytes_per_pixel;
uint16_t x_res, y_res;
uint8_t *main_frame_buffer; 
extern uint32_t interrupt_counter;

int (set_graphic_mode)(uint16_t mode){
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86_t)); // por a estrutura a 0
  reg86.intno = BIOS_VIDEOCARD_SERV; // BIOS video services
  reg86.ax = VBE_MODE_SET; // set video mode function
  reg86.bx = mode | LINEAR_FRAMEBUFFER; // set the mode and linear framebuffer
  sys_int86(&reg86); // call BIOS interrupt
  return 0;
}

int (set_frame_buffer)(uint16_t mode){
  memset(&vg_mode_info, 0, sizeof(vg_mode_info));
  vbe_get_mode_info(mode, &vg_mode_info); // get mode info

  unsigned int size = vg_mode_info.XResolution * vg_mode_info.YResolution * ((vg_mode_info.BitsPerPixel+7) / 8); // calculate size of framebuffer

  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = vg_mode_info.PhysBasePtr; // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + size; // fim físico do buffer

  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses); // add memory range to process
     
  main_frame_buffer = vm_map_phys(SELF, (void *)physic_addresses.mr_base, size); // map memory range to process

  return 0;
}

inline uint8_t *(get_position)(uint16_t x, uint16_t y, uint8_t *frame_buffer) {
  return frame_buffer + (x + x_res * y) * bytes_per_pixel; 
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer) {
  return vga_draw_hline(x, y, 1, color, frame_buffer);
}

int(vga_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer) {
  if (len == 0 || x >= x_res || y >= y_res) {
    fprintf(stderr, "vg_draw_hline: invalid coordinates/dimensions, x:%u y:%u len:%u.\n", x, y, len);
    return 1;
  }

  uint8_t *address = get_position(x, y, frame_buffer);
  for (uint16_t i = 0; i < len && x + i < x_res; i++, address += bytes_per_pixel) {
    memcpy(address, &color, bytes_per_pixel);
  }

  return 0;
}

int(vga_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer) {
  if (width == 0 || height == 0 || x >= x_res || y >= y_res) {
    fprintf(stderr, "vg_draw_rectangle: invalid coordinates/dimensions, x:%u y:%u width:%u height:%u.\n", x, y, width, height);
    return 1;
  }

  int retv = 0;
  for (uint16_t i = 0; i < height && y + i < y_res; i++)
    retv |= vga_draw_hline(x, y + i, width, color, frame_buffer);

  if (retv) {
    fprintf(stderr, "vg_draw_rectangle: vg_draw_hline failed.\n");
    return 1;
  } else {
    return 0;
  }
}

uint32_t(direct_mode)(uint16_t i, uint16_t j, uint8_t step, uint32_t first) {
  uint8_t rfp = vg_mode_info.RedFieldPosition;
  uint8_t gfp = vg_mode_info.GreenFieldPosition;
  uint8_t bfp = vg_mode_info.BlueFieldPosition;

  uint8_t rms = vg_mode_info.RedMaskSize;
  uint8_t gms = vg_mode_info.GreenMaskSize;
  uint8_t bms = vg_mode_info.BlueMaskSize;

  uint8_t r = (get_component(first, rfp, rms) + j * step) % (1 << rms);
  uint8_t g = (get_component(first, gfp, gms) + i * step) % (1 << gms);
  uint8_t b = (get_component(first, bfp, bms) + (j + i) * step) % (1 << bms);

  return (r << rfp) | (g << gfp) | (b << bfp);
}

inline uint8_t(get_component)(uint32_t color, uint8_t field_position, uint8_t mask_size) {
  return (color >> field_position) & (BIT(mask_size) - 1);
}

uint32_t(indexed_mode)(uint8_t no_rectangles, uint16_t i, uint16_t j, uint8_t step, uint32_t first) {
  return (first + (i * no_rectangles + j) * step) % (1 << vg_mode_info.BitsPerPixel);
}


