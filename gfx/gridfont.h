// NCurses grid-font
// (c) 2020 by Stefan Roettger

#pragma once

#include "gfx.h"

//! initialize grid font with character size (cols, lines)
//! * default character size is 5x3
void init_grid_font(int cols = 0, int lines = 0);

//! get character cols
int get_grid_char_cols();

//! get character lines
int get_grid_char_lines();

//! set character by data array
void set_grid_char_data(int ch, const int *data);

//! set character by text string
//! * "interprete":
//! ** ^ as overscore
//! ** # as checker board
//! ** B as bold attribute
//! ** digits as color attributes
//! ** 0 to clear attributes
void set_grid_char_text(int ch, const char *text, bool interprete = false);

//! get character data
int *get_grid_char_data(int ch);

//! release allocated memory
void release_grid_font();
