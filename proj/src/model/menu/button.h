#ifndef BUTTON_H
#define BUTTON_H

#include <lcom/lcf.h>
#include "drivers/video/gpu.h"

/**
 * @brief X-coordinate for the Play button.
 */
#define BUTTON_PLAY_START_X 212

/**
 * @brief X-coordinate for the Menu button.
 */
#define BUTTON_MENU_START_X 212

/**
 * @brief X-coordinate for the Exit button.
 */
#define BUTTON_EXIT_START_X 522

/**
 * @brief Y-coordinate for buttons in the main menu.
 */
#define MAIN_MENU_BUTTON_START_Y 500

/**
 * @brief Y-coordinate for buttons in the game over screen.
 */
#define GAME_OVER_BUTTON_START_Y 400

/**
 * @brief Width of a button in pixels.
 */
#define BUTTON_WIDTH 290

/**
 * @brief Height of a button in pixels.
 */
#define BUTTON_HEIGHT 139

/**
 * @brief Enumeration of button types.
 */
typedef enum {
    BUTTON_NONE, /**< No button selected */
    BUTTON_PLAY, /**< Play button */
    BUTTON_MENU, /**< Menu button */
    BUTTON_EXIT  /**< Exit button */
} ButtonType;

typedef struct Button Button;

/**
 * @brief Creates a new Button instance with the given properties.
 *
 * @param xpm XPM image for the button in default state.
 * @param xpm_selected XPM image for the button when selected.
 * @param x X-coordinate of the button.
 * @param y Y-coordinate of the button.
 * @return Pointer to the created Button structure.
 */
Button *create_button(xpm_map_t xpm, xpm_map_t xpm_selected, uint16_t x, uint16_t y);

/**
 * @brief Frees the memory associated with a Button.
 *
 * @param button Pointer to the Button structure to destroy.
 */
void destroy_button(Button *button);

/**
 * @brief Gets whether the button is currently selected.
 *
 * @param button Pointer to the Button.
 * @return true if the button is selected, false otherwise.
 */
bool button_get_selected(Button *button);

/**
 * @brief Sets the selected state of the button.
 *
 * @param button Pointer to the Button.
 * @param selected Boolean value indicating whether the button is selected.
 */
void button_set_selected(Button *button, bool selected);

/**
 * @brief Checks if a click at the given coordinates is within the button bounds.
 *
 * @param button Pointer to the Button.
 * @param x X-coordinate of the click.
 * @param y Y-coordinate of the click.
 * @return true if the button was clicked, false otherwise.
 */
bool button_is_clicked(Button *button, uint16_t x, uint16_t y);

/**
 * @brief Draws the button onto the given frame buffer.
 *
 * @param button Pointer to the Button.
 * @param frame_buffer Pointer to the frame buffer.
 */
void draw_button(Button *button, uint8_t *frame_buffer);

#endif /* BUTTON_H */

