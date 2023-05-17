// NCurses polygon rendering
// (c) 2022 by Stefan Roettger

#pragma once

#include "math2d.h"

//! render a polygon with n vertices
//! * the polygon can be self-intersecting
void render_polygon(int n, const Vec2 vertex[],
                    int ch = -1);

//! render a triangle with 3 vertices
void render_triangle(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3,
                     int ch = -1);
