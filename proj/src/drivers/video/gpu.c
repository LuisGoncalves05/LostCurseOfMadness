#include "gpu.h"
#include "model/utils/sprite.h"
#include <lcom/lcf.h>
#include <machine/int86.h>
#include <math.h>

xpm_image_t img;
vbe_mode_info_t vg_mode_info;
uint8_t bytes_per_pixel;
uint16_t x_res, y_res;
uint32_t frame_size;
bool frame_start = true;
uint8_t *main_frame_buffer;
uint8_t *sec_frame_buffer;

int(set_graphic_mode)(uint16_t mode) {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86_t));
  reg86.intno = BIOS_VIDEOCARD_SERV;
  reg86.ax = VBE_MODE_SET;
  reg86.bx = mode | LINEAR_FRAMEBUFFER;
  sys_int86(&reg86);
  return 0;
}

int(set_frame_buffers)(uint16_t mode) {
  memset(&vg_mode_info, 0, sizeof(vg_mode_info));
  vbe_get_mode_info(mode, &vg_mode_info); // get mode info

  x_res = vg_mode_info.XResolution;                      // get x resolution
  y_res = vg_mode_info.YResolution;                      // get y resolution
  bytes_per_pixel = (vg_mode_info.BitsPerPixel + 7) / 8; // calculate bytes per pixel

  frame_size = vg_mode_info.XResolution * vg_mode_info.YResolution * bytes_per_pixel; // calculate size of framebuffer

  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = vg_mode_info.PhysBasePtr;                   // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + 2 * frame_size; // fim físico do buffer

  sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses); // add memory range to process

  main_frame_buffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, 2 * frame_size); // map memory range to process
  sec_frame_buffer = main_frame_buffer + frame_size;

  return 0;
}

int(clear)(uint8_t *frame_buffer) {
  return memset(frame_buffer, 0, x_res * y_res * bytes_per_pixel) == NULL;
}

void(set_display_start)() {
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86_t));
  reg86.intno = BIOS_VIDEOCARD_SERV;
  reg86.ax = VBE_DISPLAY_START_SET;
  reg86.bx = VERTICAL_RETRACE;
  reg86.cx = 0;
  reg86.dx = frame_start ? 0 : y_res;
  sys_int86(&reg86);
}

void vga_flip_pages() {
  frame_start = !frame_start;
  set_display_start();
  uint8_t *tmp = main_frame_buffer;
  main_frame_buffer = sec_frame_buffer;
  sec_frame_buffer = tmp;
}

uint8_t *(get_position) (uint16_t x, uint16_t y, uint8_t *frame_buffer) {
  return frame_buffer + (x + x_res * y) * bytes_per_pixel;
}

int(vga_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer) {
  return vga_draw_hline(x, y, 1, color, frame_buffer);
}

int(vga_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer) {
  if (len == 0 || x >= x_res || y >= y_res) {
    printf("vga_draw_hline: invalid coordinates/dimensions, x:%u y:%u len:%u.\n", x, y, len);
    return 1;
  }

  uint8_t *address = get_position(x, y, frame_buffer);
  for (uint16_t i = 0; i < len && x + i < x_res; i++, address += bytes_per_pixel) {
    memcpy(address, &color, bytes_per_pixel);
  }

  return 0;
}

int(vga_draw_line)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t *frame_buffer) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;
  int e2;

  while (true) {
    // Desenhar o pixel na posição atual
    if (x1 >= 0 && x1 < x_res && y1 >= 0 && y1 < y_res) {
      vga_draw_pixel(x1, y1, color, frame_buffer);
    }

    // Verificar se chegamos ao ponto final
    if (x1 == x2 && y1 == y2)
      break;

    e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }

  return 0;
}

int(vga_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer) {
  if (width == 0 || height == 0 || x >= x_res || y >= y_res) {
    // printf("vg_draw_rectangle: invalid coordinates/dimensions, x:%u y:%u width:%u height:%u.\n", x, y, width, height);
    return 1;
  }

  int retv = 0;
  for (uint16_t i = 0; i < height && y + i < y_res; i++)
    retv |= vga_draw_hline(x, y + i, width, color, frame_buffer);

  if (retv) {
    printf("vg_draw_rectangle: vg_draw_hline failed.\n");
    return 1;
  }
  else {
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

void get_rotated_bounds(double width, double height, double theta, double *out_width, double *out_height) {
  double cos_theta = cos(theta);
  double sin_theta = sin(theta);

  double corners_x[] = {-width / 2, width / 2, width / 2, -width / 2};
  double corners_y[] = {-height / 2, -height / 2, height / 2, height / 2};

  double min_x = 1e9, max_x = -1e9, min_y = 1e9, max_y = -1e9;

  for (int i = 0; i < 4; i++) {
    double x = corners_x[i] * cos_theta - corners_y[i] * sin_theta;
    double y = corners_x[i] * sin_theta + corners_y[i] * cos_theta;

    if (x < min_x)
      min_x = x;
    if (x > max_x)
      max_x = x;
    if (y < min_y)
      min_y = y;
    if (y > max_y)
      max_y = y;
  }

  *out_width = max_x - min_x;
  *out_height = max_y - min_y;
}

int(draw_sprite)(Sprite *sprite, uint8_t *frame_buffer) {
  if (sprite == NULL) {
    printf("draw_sprite: NULL pointer provided.\n");
    return 1;
  }

  if (sprite->x >= x_res || sprite->y >= y_res) {
    printf("draw_sprite: invalid sprite position, x:%d, y:%d.\n", sprite->x, sprite->y);
    return 1;
  }

  uint8_t *map = sprite->map;

  for (int h = 0; h < sprite->height && sprite->y + h < y_res; h++)
    for (int w = 0; w < sprite->width && sprite->x + w < x_res; w++, map++)
      if (*map != BACKGROUND_COLOR)
        vga_draw_pixel(sprite->x + w, sprite->y + h, *map, frame_buffer);
      
  return 0;
}

int draw_sprite_rotated(Sprite *sprite, double theta, uint8_t *frame_buffer) {

  double rotated_width, rotated_height;
  get_rotated_bounds(sprite->width, sprite->height, theta, &rotated_width, &rotated_height);

  double center_x = sprite->width / 2.0;
  double center_y = sprite->height / 2.0;

  double cos_theta = cos(theta);
  double sin_theta = sin(theta);

  for (int i = 0; i < (int) rotated_height; i++) {
    for (int j = 0; j < (int) rotated_width; j++) {
      double translated_x = j - rotated_width / 2.0;
      double translated_y = i - rotated_height / 2.0;
      // se der merda na sync no mouse e do sprite troquem o sinal
      //                                          |
      //                                          v
      double source_x = cos_theta * translated_x + sin_theta * translated_y + center_x;
      double source_y = sin_theta * translated_x - cos_theta * translated_y + center_y;

      if (source_x >= 0 && source_x < sprite->width && source_y >= 0 && source_y < sprite->height) {
        int src_x = (int) source_x;
        int src_y = (int) source_y;

        uint8_t color = sprite->map[src_y * sprite->width + src_x];

        if (color != BACKGROUND_COLOR) {
          vga_draw_pixel(sprite->x + j - rotated_width / 2.0, sprite->y + i - rotated_height / 2.0, color, frame_buffer);
        }
      }
    }
  }
  return 0;
}

int vga_draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *frame_buffer) {
  xpm_image_t img;
  uint8_t *data = xpm_load(xpm, XPM_INDEXED, &img);
  if (x >= x_res || y >= y_res) {
    printf("vg_draw_xpm: invalid xpm position, x:%hu, y:%hu.\n", x, y);
    return 1;
  }
  uint8_t *ptr = get_position(x, y, frame_buffer);
  if (ptr == NULL) {
    fprintf(stderr, "vg_draw_xpm: get_position failed.\n");
    return 1;
  }
  
  uint16_t line_size = img.width * bytes_per_pixel;
  uint16_t usable_line_size = line_size;
  if (x + img.width > x_res)
    usable_line_size = (x_res - x) * bytes_per_pixel;

  for (int h = 0; h < img.height && y + h < y_res; h++, ptr += x_res * bytes_per_pixel, data += line_size)
    memcpy(ptr, data, usable_line_size);

  return 0;
}
