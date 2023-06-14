// NCurses side-scrolling grid-font area
// (c) 2020 by Stefan Roettger

#pragma once

#include "gfx.h"

//! create a scrollable grid area
//! * with sx columns and sy rows
//! * the grid area contains a single grid char per cell
//!  * a grid char consists of a grid of 5x3 ASCII characters by default
//! * after grid area creation we need to
//!  * define the screen window size via set_grid_window_size
//!  * define the contents of the grid area via set_grid, flood_fill_grid etc.
//!  * and finally render the grid via redraw_grid_window or scroll_grid_window
void set_grid_size(int sx, int sy);

//! get the width of the scrollable grid area
int get_grid_width();

//! get the height of the scrollable grid area
int get_grid_height();

//! set the size of the displayed grid window
//! * the size is defined in terms of grid chars
void set_grid_window_size(int sx, int sy);

//! is the grid wrapping around or not?
void set_grid_wraparound(bool yes = true);

//! clear the scrollable grid area
void clear_grid(int ch = ' ');

//! clear the scrollable extra grid area
void clear_extra(int ch = -1);

//! set the border of the scrollable grid area
void set_grid_border(int ch);

//! get the grid cell character at grid position (x, y)
int get_grid(int x, int y);

//! get the last cell character at grid position (x, y)
int get_last(int x, int y);

//! get the same cell character at grid position (x, y)
int get_same(int x, int y);

//! get the extra grid cell character at grid position (x, y)
int get_extra(int x, int y);

//! set the grid cell at grid position (x, y) to character ch
void set_grid(int x, int y, int ch);

//! set the last cell at grid position (x, y) to character ch
void set_last(int x, int y, int ch);

//! set the same cell at grid position (x, y) to character ch
void set_same(int x, int y, int ch);

//! set the extra grid cell at grid position (x, y) to character ch
void set_extra(int x, int y, int ch);

//! check grid cell area for presence of character ch
bool check_grid(int ch);

//! count character ch in grid cell area
int count_grid(int ch);

//! replace character ch in grid cell area
void replace_grid(int ch, int replace);

//! get the grid cell area at top-left position (x, y) with size (sx, sy)
//! * returns a newly allocated array of respective size
int *get_grid_area(int x, int y,
                   int sx, int sy);

//! set a grid cell area at top-left position (x, y) with size (sx, sy)
void set_grid_area(int x, int y,
                   int sx, int sy,
                   const int *data);

//! set the grid cells at top-left grid position (x, y) to a text string
void set_grid_text(int x, int y,
                   const char *text);

//! render a grid line from position (x1, y1) to (x2, y2)
void render_grid_line(int x1, int y1, int x2, int y2, int ch);

//! flood-fill a grid cell area starting at position (x, y)
void flood_fill_grid(int x, int y, int ch);

//! flood-fill everything but a grid cell area starting at position (x, y)
void inverse_flood_fill_grid(int x, int y, int ch);

//! place random characters
void place_random(int ch, int num, int exclude);

//! set character animation
void set_grid_animation(int ch, int num, const int *data);

//! set character animation string
void set_grid_animation_string(int ch, const char *str);

//! redraw the displayed grid window at center grid position (x, y)
void redraw_grid_window(int x, int y);

//! scroll the displayed grid window to center grid position (x, y)
void scroll_grid_window(int x, int y, int deltax = 0, int deltay = 0, bool stop = true);

//! release allocated memory
void release_grid();
