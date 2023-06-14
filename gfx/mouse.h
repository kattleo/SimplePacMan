// mouse input for NCurses
// (c) 2023 by Stefan Roettger

#pragma once

#include "gfx.h"

//! init mouse reporting
void mouse_init();

//! decode wide keycode
//! * if a mouse event has been observed as a wide keycode, the function decodes it and returns true
//! * in the latter case, the mouse position (x, y) and the pressed buttons are returned via call-by-reference
bool mouse_keycode(int keycode,
                   unsigned int *x, unsigned int *y,
                   unsigned int *buttons);

//! exit mouse reporting
void mouse_exit();
