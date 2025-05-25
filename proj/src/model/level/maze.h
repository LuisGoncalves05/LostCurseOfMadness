/**
 * @file maze.h
 *
 * @brief This file contains the declarations for the maze module.
 * The maze module handles maze generation, rendering, and collision detection.
 */

/** @defgroup maze Maze
 *  @brief Maze module for generating and managing the game maze.
 *  @{
 */

#ifndef MAZE_H
#define MAZE_H

#include "../../drivers/video/gpu.h"
#include <lcom/lcf.h>

#define EMPTY 0
#define WALL 1
#define MOB 2

/**
 * @brief Color used for the walls of the maze.
 */
#define WALL_COLOR 0x07

/**
 * @brief Total size of each cell including wall width.
 */
#define CELL_SIZE 52

/**
 * @brief Width of the walls in pixels.
 */
#define WALL_WIDTH 2

/**
 * @brief Margin used for collision detection with the maze walls.
 */
#define LINE_COLLISION_MARGIN 2

/**
 * @brief Width of the maze in pixels.
 */
#define WIDTH 1024

/**
 * @brief Height of the maze in pixels.
 */
#define HEIGHT 768

typedef struct Maze Maze;

typedef struct Line {
  int x1, y1;
  int x2, y2;
} Line;

typedef struct Point {
  int x, y;
} Point;

extern uint32_t frame_size;

/**
 * @brief Creates and initializes a new maze with the given dimensions.
 *
 * @param width Width of the maze in cells.
 * @param height Height of the maze in cells.
 * @param mob_count Number of mobs in the maze.
 * @return Pointer to the created Maze structure.
 */
Maze *create_maze(uint8_t width, uint8_t height, uint8_t mob_count);

/**
 * @brief Frees the memory associated with the maze.
 *
 * @param maze Pointer to the Maze structure to free.
 */
void free_maze(Maze *maze);

/**
 * @brief Draws the maze to the provided frame buffer.
 *
 * @param maze Pointer to the Maze structure to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int draw_maze(Maze *maze, uint8_t *frame_buffer);

/**
 * @brief Checks for collision between a rectangle and the maze walls.
 *
 * @param maze Pointer to the Maze structure.
 * @param x X-coordinate of the top-left corner of the rectangle.
 * @param y Y-coordinate of the top-left corner of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @return true if there is a collision, false otherwise.
 */
bool check_collision(Maze *maze, int x, int y, int width, int height);

/**
 * @brief Checks for collision between a rectangle and a specific maze wall line.
 *
 * @param x X-coordinate of the top-left corner of the rectangle.
 * @param y Y-coordinate of the top-left corner of the rectangle.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param line The Line structure representing a maze wall.
 * @return true if there is a collision, false otherwise.
 */
bool check_line_collision(int x, int y, int width, int height, Line line);

/**
 * @brief Initializes the buffer used for maze drawing.
 */
void init_maze_buffer();

/**
 * @brief Frees the buffer used for maze drawing.
 */
void free_maze_buffer();

/**
 * @brief Returns the width of the maze in cells.
 *
 * @param maze Pointer to the Maze structure.
 * @return Width of the maze.
 */
uint8_t get_width(Maze *maze);

/**
 * @brief Returns the height of the maze in cells.
 *
 * @param maze Pointer to the Maze structure.
 * @return Height of the maze.
 */
uint8_t get_height(Maze *maze);

/**
 * @brief Returns the number of lines in the maze (used for rendering and collision).
 *
 * @param maze Pointer to the Maze structure.
 * @return Number of lines in the maze.
 */
int get_line_count(Maze *maze);

/**
 * @brief Returns the positions in the maze where there are mobs.
 *
 * @param maze Pointer to the Maze structure.
 * @return Point array of mob positions;
 */
Point **get_mob_positions(Maze *maze);

/**
 * @brief Returns the number of mobs in the maze.
 *
 * @param maze Pointer to the Maze structure.
 * @return Number of mobs in the maze.
 */
uint8_t get_mob_count(Maze *maze);

#endif

/** @} */
