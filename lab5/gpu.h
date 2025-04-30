#ifndef GPU_H
#define GPU_H

#include <lcom/vbe.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup gpu gpu
 * @brief Functions and constants for handling Gpu operations.
 * @{
 */

/** @name VBE BIOS Function Codes */
/** @{ */
#define VBE_CONTROLLER_INFO 0x4F00 /**< Request VBE controller information. */
#define VBE_MODE_INFO 0x4F01       /**< Request VBE mode information. */
#define VBE_MODE_SET 0x4F02        /**< Set a VBE graphics mode. */
#define BIOS_VIDEO_MODE_SET 0x0003 /**< Reset to Minix default text mode. */
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
#define VBE 0x10                   /**< BIOS video interrupt number. */
#define DIRECT_MODE 6              /**< Direct color memory model identifier. */
#define BACKGROUND_COLOR 0         /**< Default background color used for clearing screen or erasing sprites. */

/**
 * @brief A drawable sprite.
 */
typedef struct {
  uint16_t x, y;          /**< Top-left corner position on screen. */
  uint16_t width, height; /**< Dimensions of the sprite in pixels. */
  uint8_t *data;          /**< Pointer to raw pixel data (XPM-loaded). */
} Sprite;

/**
 * @brief Creates a new sprite from an XPM image.
 *
 * Allocates memory and loads the image into a Sprite structure,
 * without drawing it to the screen.
 *
 * @param xpm Pointer to XPM map.
 * @param x Initial x position.
 * @param y Initial y position.
 * @return Pointer to the created Sprite, or NULL on error.
 */
Sprite *create_sprite(xpm_map_t xpm, int x, int y);

/**
 * @brief Sets the VBE graphics mode.
 *
 * @param mode VBE mode number to set (e.g., VBE_MODE1).
 * @return 0 on success, non-zero on failure.
 */
int(vg_set_vbe_mode)(uint16_t mode);

/**
 * @brief Returns a pointer to the video memory location of a specific pixel.
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @return Pointer to the memory location for pixel (x, y).
 */
static inline uint8_t *(get_position) (uint16_t x, uint16_t y);

int8_t(get_direction)(int16_t i, int16_t f);

/**
 * @brief Draws a single pixel of a specified color at (x, y).
 *
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param color Color to draw.
 * @return 0 on success, non-zero on failure.
 */
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a full pattern using colored rectangles.
 *
 * Uses either indexed or direct color mode depending on the video mode.
 *
 * @param no_rectangles Number of rectangles per row/column.
 * @param first Base color.
 * @param step Color increment step.
 * @return 0 on success, non-zero on failure.
 */
int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step);

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

/**
 * @brief Draws an XPM image at a specific screen position.
 *
 * Internally converts to sprite and draws it.
 *
 * @param xpm Pointer to XPM image.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @return 0 on success, non-zero on failure.
 */
int(vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Renders a previously loaded sprite at its current position.
 *
 * @param sprite Pointer to sprite.
 * @return 0 on success, non-zero on failure.
 */
int(vg_draw_sprite)(Sprite *sprite);

/**
 * @brief Calculates direction of movement between two coordinates.
 *
 * @param i Initial coordinate.
 * @param f Final coordinate.
 * @return -1 (negative direction), 1 (positive direction), or 0 (no movement).
 */
int8_t(get_direction)(int16_t i, int16_t f);

/**
 * @brief Updates the position of a sprite and redraws it accordingly.
 *
 * Handles both fixed-speed and frame-based motion (positive and negative speed).
 *
 * @param sprite Pointer to sprite.
 * @param xf Final x coordinate.
 * @param yf Final y coordinate.
 * @param speed Movement speed (pixels/frame if positive; frames/pixel if negative).
 * @param dir Direction of movement (-1 or 1).
 * @param movement_x True if moving horizontally, false if vertically.
 * @return 0 on success, non-zero on failure.
 */
int(vg_update_sprite)(Sprite *sprite, uint16_t xf, uint16_t yf, int16_t speed, int8_t dir, bool movement_x);

/** @} */
#endif /* GPU_H */
