// NCurses graphics
// (c) 2020 by Stefan Roettger

#pragma once

#include <ncurses.h> // for NCurses

#include <math.h> // for math functions
#include <ctype.h> // for c character definitions
#include <string.h> // for c string definitions
#include <stdio.h> // for c std i/o functions
#include <stdlib.h> // for standard c functions
#include <stdbool.h> // for standard bool values
#include <unistd.h> // for posix c functions
#include "math2d.h" // for 2D vector math functions
#include "util.h" // for utility functions

//! init ASCII GFX
//! * setup the standard screen of NCurses
//! * the screen origin (0, 0) is at the top left corner
void init_gfx();

//! init ASCII GFX color display
//! * default colors are white, red, green, blue with indices 1-4
void init_color();

//! exit ASCII GFX
//! * close the standard screen of NCurses
void exit_gfx();

//! set the drawing window
//! * by default the standard screen is used
void set_window(WINDOW *w);

//! use indexed color pair for foreground/background colors of characters
//! * 1 = white/black
//! * 2 = red/black
//! * 3 = green/black
//! * 4 = blue/black
void use_color(int index = 1);

//! use attribute bold
void use_attr_bold();

//! use attribute blink
void use_attr_blink();

//! use attribute reverse
void use_attr_reverse();

//! use attribute normal
void use_attr_normal();

//! draw a formatted text at position (x, y)
//! * non-printable characters like \\n will be ignored
void draw_text(int y, int x,
               const char *format, ...);

//! draw a sprite at barycenter position (x, y)
//! * the sprite text needs to be a string that is terminated by \\0
//! * the sprite text is made up from consecutive text lines separated by \\n
//! * if the sprite position is outside of the screen the text will wrap around
void draw_sprite(int y, int x,
                 const char *text);

//! draw a screen area at top-left position (x, y) with size (sx, sy)
//! * the data is made up from consecutive lines of characters
void draw_area(int y, int x,
               int sy, int sx,
               const int *data);

//! draw a text string with grid font characters at top-left position (x, y)
//! * the grid font needs to be initialized beforehand via init_grid_font()
void draw_grid_text(int y, int x,
                    const char *text);

//! draw a line from position (x1, y1) to (x2, y2)
//! * symmetrified Bresenham algorithm
//! * "ch" is the character used to draw the line
void draw_line(int y1, int x1, int y2, int x2,
               int ch = -1);

//! draw a frame from position (y1, x1) to (y2, x2)
//! * "ch" is the character used to draw the frame
void draw_frame(int y1, int x1, int y2, int x2,
                int ch = -1);

//! draw a circle at center position (xc, yc) with radius r
//! * Bresenham's circle drawing algorithm
//! * "ch" is the character used to draw the circle
void draw_circle(int yc, int xc, int r,
                 int ch = -1, double aspect = 2);

//! draw a ellipse at center position (xc, yc) with principle axis ax and ay
//! * Bresenham's ellipse drawing algorithm
//! * "ch" is the character used to draw the ellipse
void draw_ellipse(int yc, int xc, int ay, int ax,
                  int ch = -1, double aspect = 2);

//! peek at location (y, x) and return the displayed character
int peek(int y, int x);

//! return wide keycode
int get_keycode();

//! decode wide keycode
//! * if a cursor event has been observed as a wide keycode, the function decodes it
//! * the function returns a corresponding direction vector (dx, dy) via call-by-reference
void cursor_keycode(int keycode,
                    int *dx, unsigned int *dy);

//! helper: allocate string buffer
//! * provides a buffer for a string with at most n characters
//! * it allocates memory for the n characters plus the terminal nul character
char *get_string_buffer(int n);
