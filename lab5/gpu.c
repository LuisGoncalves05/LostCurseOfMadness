#include <lcom/lcf.h>
#include <machine/int86.h>
#include "gpu.h"

static vbe_mode_info_t vg_mode_info;
static uint8_t bytes_per_pixel;
static uint16_t x_res, y_res;
static uint8_t *video_mem;
extern uint32_t interrupt_counter;

Sprite *create_sprite(xpm_map_t xpm, int x, int y) {
  Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));
  xpm_image_t img;
  if (sprite == NULL) {
    fprintf(stderr, "create_sprite: malloc failed.\n");
    return NULL;
  }

  sprite->data = xpm_load(xpm, XPM_INDEXED, &img);
  if (sprite->data == NULL) {
    free(sprite);
    fprintf(stderr, "create_sprite: xpm_load failed.\n");
    return NULL;
  }
  sprite->x = x;
  sprite->y = y;
  sprite->width = img.width;
  sprite->height = img.height;
  return sprite;
}

int(vg_set_vbe_mode)(uint16_t mode) {
  reg86_t registers;
  memset(&registers, 0, sizeof(registers));
  registers.ax = VBE_MODE_SET;
  registers.bx = LINEAR_FRAMEBUFFER | mode;
  registers.intno = VBE;
  if (sys_int86(&registers) != 0) {
    fprintf(stderr, "vbe_set_mode: sys_int86() failed.\n");
    return 1;
  }
  return 0;
}

void *(vg_init) (uint16_t mode) {
  if (vbe_get_mode_info(mode, &vg_mode_info)) {
    fprintf(stderr, "vg_init: vbe_get_mode_info failed.\n");
    return NULL;
  }

  struct minix_mem_range mr; /* physical memory range */
  phys_bytes vram_base = vg_mode_info.PhysBasePtr; /* VRAM’s physical addresss */
  bytes_per_pixel = (vg_mode_info.BitsPerPixel + 7) / 8; /* bits_per_pixel / 8 + ((bits_per_pixel % 8) != 0) */
  x_res = vg_mode_info.XResolution;
  y_res = vg_mode_info.YResolution;
  unsigned int vram_size = x_res * y_res * bytes_per_pixel; /* VRAM’s size, but you can use the frame-buffer size, instead */

  mr.mr_base = vram_base;
  mr.mr_limit = vram_base + vram_size;

  int retv;
  if (0 != (retv = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", retv);

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  if (vg_set_vbe_mode(mode)) {
    fprintf(stderr, "vg_init: vg_set_vbe_mode failed.\n");
    return NULL;
  } 
  
  return video_mem;
}

inline uint8_t *(get_position) (uint16_t x, uint16_t y) {
  return video_mem + (x + x_res * y) * bytes_per_pixel;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  return vg_draw_hline(x, y, 1, color);
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  if (len == 0 || x >= x_res || y >= y_res) {
    fprintf(stderr, "vg_draw_hline: invalid coordinates/dimensions, x:%u y:%u len:%u.\n", x, y, len);
    return 1;
  }

  uint8_t *address = get_position(x, y);
  for (uint16_t i = 0; i < len && x + i < x_res; i++, address += bytes_per_pixel) {
    memcpy(address, &color, bytes_per_pixel);
  }

  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (width == 0 || height == 0 || x >= x_res || y >= y_res) {
    fprintf(stderr, "vg_draw_rectangle: invalid coordinates/dimensions, x:%u y:%u width:%u height:%u.\n", x, y, width, height);
    return 1;
  }

  int retv = 0;
  for (uint16_t i = 0; i < height && y + i < y_res; i++)
    retv |= vg_draw_hline(x, y + i, width, color);

  if (retv) {
    fprintf(stderr, "vg_draw_rectangle: vg_draw_hline failed.\n");
    return 1;
  } else 
    return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (no_rectangles > x_res || no_rectangles > y_res) {
    fprintf(stderr, "vg_draw_pattern: invalid no_rectangles %d.\n", no_rectangles);
    return 1;
  }
  uint16_t width = x_res / no_rectangles;
  uint16_t height = y_res / no_rectangles;

  int retv = 0;
  for (int i = 0; i < no_rectangles; i++) {
    for (int j = 0; j < no_rectangles; j++) {
      uint32_t color = vg_mode_info.MemoryModel == DIRECT_MODE ? direct_mode(i, j, step, first) : indexed_mode(no_rectangles, i, j, step, first);
      retv |= vg_draw_rectangle(j * width, i * height, width, height, color);
    }
  }
  if (retv) {
    fprintf(stderr, "vg_draw_pattern: vg_draw_rectangle failed.\n");
    return 1;
  } else 
    return 0;
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

int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  Sprite *sprite = create_sprite(xpm, x, y);
  if (sprite == NULL) {
    fprintf(stderr, "vg_draw_xpm: create_sprite failed.\n");
    return 1;
  }

  if (vg_draw_sprite(sprite)) {
    fprintf(stderr, "vg_draw_xpm: vg_draw_sprite failed.\n");
    return 1;
  }

  return 0;
}

int(vg_draw_sprite)(Sprite *sprite) {
  if (sprite == NULL) {
    fprintf(stderr, "vg_draw_sprite: NULL pointer provided.\n");
    return 1;
  }

  if (sprite->x >= x_res || sprite->y >= y_res) {
    fprintf(stderr, "vg_draw_sprite: invalid sprite position, x:%hu, y:%hu.\n", sprite->x, sprite->y);
    return 1;
  }

  uint16_t line_size = sprite->width * bytes_per_pixel;
  uint16_t usable_line_size = line_size;
  uint8_t *data = sprite->data;
  uint8_t *ptr = get_position(sprite->x, sprite->y);
  if (ptr == NULL) {
    fprintf(stderr, "vg_draw_sprite: get_position failed.\n");
    return 1;
  }

  if (sprite->x + sprite->width > x_res)
    usable_line_size = (x_res - sprite->x) * bytes_per_pixel;

  for (int h = 0; h < sprite->height && sprite->y + h < y_res; h++, ptr += x_res * bytes_per_pixel, data += line_size)
    memcpy(ptr, data, usable_line_size);

  return 0;
}

int8_t(get_direction)(int16_t i, int16_t f) {
  int16_t dir = (f - i);
  if (dir)
    dir /= abs(dir); /* -1, 0, 1 indicating movement direction in x axis */
  return dir;
}

int(vg_update_sprite)(Sprite *sprite, uint16_t xf, uint16_t yf, int16_t speed, int8_t dir, bool movement_x) {
  if (sprite == NULL) {
    fprintf(stderr, "vg_update_sprite: NULL pointer provided.\n");
    return 1;
  }

  if (sprite->x >= x_res || sprite->y >= y_res) {
    fprintf(stderr, "vg_update_sprite: invalid position x:%hu y:%hu.\n", sprite->x, sprite->y);
    return 1;
  }

  if (speed == 0 || dir == 0) {
    fprintf(stderr, "vg_update_sprite: invalid speed or direction (0).\n");
    return 1;
  }

  if (vg_draw_rectangle(sprite->x, sprite->y, sprite->width, sprite->height, BACKGROUND_COLOR)) { /* erase sprite from the previous place */
    fprintf(stderr, "vg_update_sprite: vg_draw_rectangle failed.\n");
    return 1;
  }

  if (speed > 0) {
    sprite->x += speed * movement_x * dir;
    sprite->y += speed * !movement_x * dir;
  }
  else if (speed < 0) {
    sprite->x += (interrupt_counter % -speed == 0) * movement_x * dir;
    sprite->y += (interrupt_counter % -speed == 0) * !movement_x * dir;
  }

  if (dir > 0) {
    sprite->x = MIN(sprite->x, xf);
    sprite->y = MIN(sprite->y, yf);
  }
  else if (dir < 0) {
    sprite->x = MAX(sprite->x, xf);
    sprite->y = MAX(sprite->y, yf);
  }

  if (vg_draw_sprite(sprite)) {
    fprintf(stderr, "vg_update_sprite: vg_draw_sprite failed.\n");
    return 1;
  }

  return 0;
}
