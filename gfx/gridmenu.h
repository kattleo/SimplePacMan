// NCurses grid-font menu
// (c) 2020 by Stefan Roettger

#pragma once

#include "gfx.h"

//! set the size of the grid-font menu
void set_menu_size(int sx, int sy);

//! clear the grid-font menu
void clear_menu(int ch = -1);

//! set the menu cell at grid position (x, y) to character ch
void set_menu(int x, int y, int ch);

//! update the selected menu cell by moving the selection cursor by a position delta
void update_menu_selection(int dx, int dy);

//! get the selected menu cell
int get_menu_selection();

//! create a menu overlay
//! * "num" is the number of the sprite overlay
//! * "x" and "y" is the top-left position (x, y) of the sprite overlay
void create_menu_overlay(int num, int x, int y);

//! release allocated memory
void release_menu();
