#pragma once

#include <stddef.h>

// needs the typedef xpm_row_t
#include "xpm.h"

/** @defgroup xpm xpm
 * @{
 *  @author Joao Cardoso (jcard@fe.up.pt)
 */

/** Format of a xpm-like (there is a real xpm format) pic:
 * <pre>
 * static xpm_row_t const picture_name[] = {
 * "number_of_x_pixels number_of_y_pixels number_of_colors",
 * "one_char_symbol the_char_symbol_color",
 * ... exactly number_of_colors lines as above
 * "any of the above one_char_symbol, exactly number_of_x_pixels times",
 * ... exactly number_of_y_pixels lines as above
 * };
 *
 * Example:
 *
 * static xpm_row_t const pic1[] = {   // the name of the picture, "pic1" in this case
 * "32 13 4",                          // number of pixels in the x and y dimension,
 *                                        and the number of colors, which follows.
 * ". 0",                              // mapping between symbols and colors; in this
 *                                        case, the "." will be mapped to color 0
 * "x 2",                              // and the "x" to color 2
 * ...                                 // the next symbol/color pairs
 * "................................", // first row, exactly 32 pixels, 
 *                                        all color 0, BLACK
 * "..............xxx...............", // next row has three GREEN pixels
 *                                        at the center
 * </pre>
 * Any real xpm picture can be read, as long as there are only 16
 * colors on it, and the colors are the ones available by default.
 *
 *  To use other/more colors than the basic ones, the graphic card
 *  palette must be programmed.
 * 
 */


static xpm_row_t const cross[] = {
  "32 32 2",
  "  0",
  "+ 13",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "             ++ ++              ",
  "            +     +             ",
  "           +       +            ",
  "+++++++++++         ++++++++++++",
  "           +       +            ",
  "            +     +             ",
  "             ++ ++              ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
  "               +                ",
};

