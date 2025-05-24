/**
 * @file game.h
 *
 * @brief This file contains the declarations for the main game module.
 * The game module manages the core game loop, game state, input handling, and transitions between menus and levels.
 */

/** @defgroup game Game
 *  @brief Main game module for managing game logic, input, and state transitions.
 *  @{
 */

#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <math.h>
#include <stdint.h>

#include "model/level/level.h"
#include "model/menu/game_over.h"
#include "model/menu/main_menu.h"
#include "model/utils/cursor.h"
#include "model/utils/keys.h"
#include "model/utils/sprite.h"

#include "drivers/keyboard/keyboard.h"
#include "drivers/mouse/mouse.h"
#include "drivers/timer/i8254.h"
#include "drivers/video/gpu.h"

#include "assets/xpm/cursor.xpm"

/**
 * @brief Radius of the player's field of view.
 */
#define FOV_RADIUS 600

extern uint8_t *maze_buffer;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res;
extern uint16_t y_res;
extern uint32_t frame_size;

extern uint8_t scan_code;
extern struct packet pp;

typedef struct Game Game;

/**
 * @brief Enum representing the different game states.
 */
typedef enum {
  MENU,       /**< Main menu state */
  LEVEL,      /**< Active game level state */
  VICTORY,    /**< Game victory state */
  GAME_OVER,  /**< Game over state */
  EXIT        /**< Exit state */
} State;

/**
 * @brief Creates and initializes a new Game instance.
 *
 * @return Pointer to the created Game structure.
 */
Game *create_game();

/**
 * @brief Frees the memory associated with the Game instance.
 *
 * @param game Pointer to the Game structure to destroy.
 */
void destroy_game(Game *game);

/**
 * @brief Returns the current state of the game.
 *
 * @param game Pointer to the Game structure.
 * @return Current game state.
 */
State get_state(Game *game);

/**
 * @brief Handles timer interrupts and updates game logic accordingly.
 *
 * @param game Pointer to the Game structure.
 */
void game_timer_handler(Game *game);

/**
 * @brief Handles keyboard input events and updates game state.
 *
 * @param game Pointer to the Game structure.
 */
void game_keyboard_handler(Game *game);

/**
 * @brief Handles mouse input events and updates game state.
 *
 * @param game Pointer to the Game structure.
 */
void game_mouse_handler(Game *game);

#endif

/** @} */
