/**
 * @file level.h
 *
 * @brief This file contains the declarations for the level module.
 * The level module encapsulates the elements that compose a game level, such as the maze and the player.
 */

/** @defgroup level Level
 *  @brief Level module for managing individual game levels.
 *  @{
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <lcom/lcf.h>

#include "maze.h"
#include "player.h"

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

#endif

/** @} */
