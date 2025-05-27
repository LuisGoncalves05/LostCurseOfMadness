#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "button.h"
#include "assets/xpm/main_menu/exit_button.xpm"
#include "assets/xpm/main_menu/exit_button_selected.xpm"
#include "assets/xpm/main_menu/lcom.xpm"
#include "assets/xpm/main_menu/play_button.xpm"
#include "assets/xpm/main_menu/play_button_selected.xpm"

extern uint16_t x_res, y_res;

/**
 * @brief Represents a single MainMenu instance.
 *
 * The MainMenu struct is opaque.
 */
typedef struct MainMenu MainMenu;

/**
 * @brief Creates and initializes a new MainMenu instance.
 *
 * @return Pointer to the created MainMenu structure.
 */
MainMenu *create_main_menu();

/**
 * @brief Frees the memory associated with the MainMenu instance.
 *
 * @param main_menu Pointer to the MainMenu structure to destroy.
 */
void destroy_main_menu(MainMenu *main_menu);

/**
 * @brief Gets the currently selected button in the main menu.
 *
 * @param main_menu Pointer to the MainMenu structure.
 * @return The type of the currently selected button.
 */
ButtonType main_menu_get_button(MainMenu *main_menu);

/**
 * @brief Cycles to the next selectable button in the main menu.
 *
 * @param main_menu Pointer to the MainMenu structure.
 */
void main_menu_change_button(MainMenu *main_menu);

/**
 * @brief Handles a mouse click and determines if a button was clicked.
 *
 * @param main_menu Pointer to the MainMenu structure.
 * @param x X-coordinate of the mouse click.
 * @param y Y-coordinate of the mouse click.
 * @return The type of the button that was clicked, or BUTTON_NONE if no button was clicked.
 */
ButtonType main_menu_click_handler(MainMenu *main_menu, uint16_t x, uint16_t y);

/**
 * @brief Draws the main menu on the given frame buffer.
 *
 * @param main_menu Pointer to the MainMenu structure.
 * @param frame_buffer Pointer to the frame buffer.
 */
void draw_main_menu(MainMenu *main_menu, uint8_t *frame_buffer);

#endif /* MAIN_MENU_H */

