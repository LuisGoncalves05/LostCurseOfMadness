#ifndef GPU_H
#define GPU_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "model/utils/sprite.h"

/**
 * @defgroup gpu gpu
 * @brief Functions and constants for handling Gpu operations.
 * @{
 */

/** @name VBE BIOS Function Codes */
/** @{ */
#define VBE_CONTROLLER_INFO 0x4F00   /**< Request VBE controller information. */
#define VBE_MODE_INFO 0x4F01         /**< Request VBE mode information. */
#define VBE_MODE_SET 0x4F02          /**< Set a VBE graphics mode. */
#define VBE_DISPLAY_START_SET 0x4F07 /**< Set a vram region to display. */
#define BIOS_VIDEO_MODE_SET 0x0003   /**< Reset to Minix default text mode. */
#define BIOS_VIDEOCARD_SERV 0x10     /**< BIOS video services. */
/** @} */

/** @name Supported VBE Modes */
/** @{ */
#define VBE_MODE1 0x105 /**< 1024x768, 256 colors (8 bpp, indexed color). */
#define VBE_MODE2 0x110 /**< 640x480, 15 bpp (direct color, RGB 5:5:5). */
#define VBE_MODE3 0x115 /**< 800x600, 24 bpp (direct color, RGB 8:8:8). */
#define VBE_MODE4 0x11A /**< 1280x1024, 16 bpp (direct color, RGB 5:6:5). */
#define VBE_MODE5 0x14C /**< 1152x864, 32 bpp (direct color, RGB 8:8:8). */
/** @} */

/** @name VBE Return Status Codes */
/** @{ */
#define CALL_FAIL 0x01            /**< Function call failed. */
#define UNSUPPORTED_FUNCTION 0x02 /**< Function not supported by hardware. */
#define INVALID_FUNCTION 0x03     /**< Invalid function call. */
/** @} */

#define LINEAR_FRAMEBUFFER BIT(14) /**< Enable linear framebuffer mode. */
#define VERTICAL_RETRACE 0x0080    /**< Set display start during vertical retrace. */
#define VBE 0x10                   /**< BIOS video interrupt number. */
#define DIRECT_MODE 6              /**< Direct color memory model identifier. */
#define BACKGROUND_COLOR 0         /**< Default background color used for clearing screen or erasing sprites. */

extern uint32_t interrupt_counter;

/**
 * @brief Sets the VBE graphics mode.
 *
 * @param mode VBE mode number to set (e.g., VBE_MODE1).
 * @return 0 on success, non-zero on failure.
 */
int(set_graphic_mode)(uint16_t mode);

/**
 * @brief Initializes the frame buffers.
 *
 * @param mode VBE mode to be used.
 * @return 0 on success, non-zero on failure.
 */
int(set_frame_buffers)(uint16_t mode);

/**
 * @brief Returns a pointer to the video memory location of a specific pixel.
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param frame_buffer Pointer to the frame buffer.
 * @return Pointer to the memory location for pixel (x, y).
 */
uint8_t *(get_position) (uint16_t x, uint16_t y, uint8_t *frame_buffer);

/**
 * @brief Draws a single pixel of a specified color at (x, y).
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a horizontal line of a specified color.
 *
 * @param x X-coordinate of the starting point.
 * @param y Y-coordinate of the line.
 * @param len Length of the line.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a line connecting two points.
 *
 * @param x1 X-coordinate of the starting point.
 * @param y1 Y-coordinate of the starting point.
 * @param x2 X-coordinate of the ending point.
 * @param y2 Y-coordinate of the ending point.
 * @param color Color of the line.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_line)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Draws a rectangle of a specified color.
 *
 * @param x X-coordinate of the top-left corner.
 * @param y Y-coordinate of the top-left corner.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int(vga_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *frame_buffer);

/**
 * @brief Extracts a specific color component from a full 32-bit color value.
 *
 * @param color Original 32-bit color.
 * @param field_position Bit position of the component.
 * @param mask_size Size in bits of the component.
 * @return Extracted component value.
 */
inline uint8_t(get_component)(uint32_t color, uint8_t field_position, uint8_t mask_size);

/**
 * @brief Calculates the color for a rectangle in direct color mode.
 *
 * @param i Row index.
 * @param j Column index.
 * @param step Color step value.
 * @param first Base color.
 * @return Computed color value.
 */
uint32_t(direct_mode)(uint16_t i, uint16_t j, uint8_t step, uint32_t first);

/**
 * @brief Calculates the color for a rectangle in indexed color mode.
 *
 * @param no_rectangles Total rectangles per row/column.
 * @param i Row index.
 * @param j Column index.
 * @param step Color increment step.
 * @param first Initial color value.
 * @return Computed color.
 */
uint32_t(indexed_mode)(uint8_t no_rectangles, uint16_t i, uint16_t j, uint8_t step, uint32_t first);

void get_rotated_bounds(double width, double height, double theta, double *out_width, double *out_height);

int draw_sprite(Sprite *sprite, uint8_t *frame_buffer);
int draw_sprite_rotated(Sprite *sprite, double theta, uint8_t *frame_buffer);

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *frame_buffer);

int(clear)(uint8_t *frame_buffer);

void(set_display_start)();

void vga_flip_pages();

#endif /* GPU_H */
