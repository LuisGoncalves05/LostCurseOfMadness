#ifndef MAZE_H
#define MAZE_H

#include "../../drivers/video/gpu.h"
#include "../utils/sprite.h"
#include <lcom/lcf.h>

#define EMPTY 0                 /**< Represents an empty cell in the maze. */
#define WALL 1                  /**< Represents a wall cell in the maze. */
#define MOB 2                   /**< Represents a mob cell in the maze. */
#define WALL_COLOR 7         /**< Color used for the walls of the maze. */
#define CELL_SIZE 64            /**< Total size of each cell including wall width. */

extern uint8_t* secondary_frame_buffer;
extern uint32_t frame_size;
extern uint16_t x_res, y_res;

/**
 * @brief Represents a single Maze instance.
 *
 * The Maze struct is opaque.
 */
typedef struct Maze Maze;

/**
 * @brief Represents a single Line instance.
 */
typedef struct Line {
    int x1, y1;
    int x2, y2;
} Line;

/**
 * @brief Represents a single Point instance.
 */
typedef struct Point {
    int x, y;
} Point;

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
void destroy_maze(Maze *maze);

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
 * @brief Returns the number of mobs in the maze.
 *
 * @param maze Pointer to the Maze structure.
 * @return Number of mobs in the maze.
 */
uint8_t get_mob_count(Maze *maze);

/**
 * @brief Sets the number of mobs in the maze.
 *
 * @param maze Pointer to the Maze structure.
 */
void set_mob_count(Maze *maze, uint8_t mob_count);

/**
 * @brief Returns the positions in the maze where there are mobs.
 *
 * @param maze Pointer to the Maze structure.
 * @return Pointer to array of mob Position pointers;
 */
Point **get_mob_positions(Maze *maze);

/**
 * @brief Checks for collision between two sprites.
 *
 * @param a Sprite a.
 * @param b Sprite b.
 * @return true if there is a collision, false otherwise.
 */
bool(check_sprite_collision)(Sprite *a, Sprite *b);

/**
 * @brief Checks for collision between a sprite and the maze walls.
 *
 * @param maze Pointer to the Maze structure.
 * @param x X-coordinate of the top-left corner of the rectangle.
 * @param y Y-coordinate of the top-left corner of the rectangle.
 * @param sprite Pointer to the Sprite structure.
 * @return true if there is a collision, false otherwise.
 */
bool(check_wall_collision)(Maze *maze, Sprite *sprite);

/**
 * @brief Draws the maze to the provided frame buffer.
 *
 * @param maze Pointer to the Maze structure to draw.
 * @param frame_buffer Pointer to the frame buffer.
 * @return 0 on success, non-zero on failure.
 */
int draw_maze(Maze *maze, uint8_t *frame_buffer);

#endif /* MAZE_H */
