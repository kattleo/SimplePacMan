// SDL joystick input for NCurses
// (c) 2020 by Stefan Roettger

#pragma once

//! init joystick
bool joystick_init();

//! joystick detected
bool joystick_detected();

//! configure joystick
void configure_joystick(bool on);

//! configure joystick axis #1
void configure_axis1(int axis);

//! configure inverted joystick axis #1
void configure_inverted1(bool invert = true);

//! configure joystick axis #2
void configure_axis2(int axis);

//! configure inverted joystick axis #2
void configure_inverted2(bool invert = true);

//! configure fire button #1
void configure_fire1(int button);

//! configure fire button #2
void configure_fire2(int button);

//! joystick moved left
bool joystick_left();

//! joystick moved right
bool joystick_right();

//! joystick moved up
bool joystick_up();

//! joystick moved down
bool joystick_down();

//! joystick fire button 1 pressed
bool joystick_fire1();

//! joystick fire button 2 pressed
bool joystick_fire2();

// decode joystick state
void joystick_state(int *dx, int *dy, int *buttons);

//! exit SDL
void joystick_exit();
