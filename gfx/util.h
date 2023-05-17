// NCurses utility functions
// (c) 2020 by Stefan Roettger

#pragma once

#include <stddef.h>
#include "math2d.h"

//! sleep for a period of time given in milli seconds
void msleep(float ms);

//! generate a random float number in the range [0,1[
float rnd();

//! check for a repeating time period
//! * "time" is the actual time
//! * "period" is the repeating time period
//! * return value is true for the first "percent" of each period
//! * by default the return value is true for the first half of the period
bool is_repeating(float time, float period,
                  float percent = 50);

//! check if a string starts with a prefix
//! * return value is eqivalent to strncmp
int strpre(const char *pre, const char *str);

//! retrieve the n-th argument from the command line arguments
//! * options starting with - or -- are omitted
const char *get_arg(int n, int argc, char *argv[]);

//! retrieve the n-th option from the command line arguments
//! * arguments not starting with - or -- are omitted
//! * if the option contains a = followed by a decimal value,
//! * then the value is returned by reference,
//! * else a default value is returned
const char *get_opt(int n, int argc, char *argv[],
                    double *value = NULL, double default_value = 0);

//! print log text in overlay
//! * log text is given in printf format style
void log_text(const char *format, ...);

//! print 2D vector to log overlay
void log_vec2(const Vec2 &v);

//! print 3D vector to log overlay
void log_vec3(const Vec3 &v);

//! print 3x3 matrix to log overlay
void log_mat3(const Mat3 &m);

//! clear log overlay
void log_clear();
