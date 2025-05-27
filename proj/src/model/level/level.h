#ifndef LEVEL_H
#define LEVEL_H

#include <lcom/lcf.h>

#include "bullet.h"
#include "maze.h"
#include "mob.h"
#include "player.h"

extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;
extern uint8_t bytes_per_pixel;
extern uint16_t x_res;
extern uint16_t y_res;

/**
 * @brief Represents a single Level instance.
 *
 * The Level struct is opaque.
 */
typedef struct Level Level;

/**
 * @brief Creates a new level instance with a specified level number.
 *
 * @param number The identifier number for the level.
 * @return Pointer to the created Level structure.
 */
Level *create_level(uint8_t number);

/**
 * @brief Frees the memory associated with the level.
 *
 * @param level Pointer to the Level structure to destroy.
 */
void destroy_level(Level *level);

/**
 * @brief Returns the Maze associated with the given level.
 *
 * @param level Pointer to the Level structure.
 * @return Pointer to the Maze structure.
 */
Maze *get_maze(Level *level);

/**
 * @brief Returns the Player associated with the given level.
 *
 * @param level Pointer to the Level structure.
 * @return Pointer to the Player structure.
 */
Player *get_player(Level *level);

/**
 * @brief Returns the delta associated with the given level.
 *
 * @param level Pointer to the Level structure.
 * @return The level's delta.
 */
double get_delta(Level *level);

/**
 * @brief Returns the Mob array associated with the given level.
 *
 * @param level Pointer to the Level structure.
 * @return Pointer to the Mob array.
 */
Mob **get_mobs(Level *level);


/**
 * @brief Updates the player's aim direction based on mouse coordinates.
 *
 * @param level Pointer to the Level structure.
 * @param mouse_x X coordinate of the mouse cursor.
 * @param mouse_y Y coordinate of the mouse cursor.
 */
void level_update_delta(Level *level, double mouse_x, double mouse_y);

/**
 * @brief Processes keyboard input to move the player.
 *
 * @param level Pointer to the Level structure.
 * @param scan_code Scan code from keyboard input.
 */
void level_update_position(Level *level, uint8_t scan_code);

/**
 * @brief Triggers a projectile from the player's position and direction.
 *
 * @param level Pointer to the Level structure.
 */
void level_shoot(Level *level);

/**
 * @brief Renders the entire level state including player, mobs, maze, and bullets.
 *
 * @param level Pointer to the Level structure.
 * @param pp Mouse packet used for determining cursor or aim direction.
 */
void draw_level(Level *level, struct packet pp);

#endif /* LEVEL_H */
