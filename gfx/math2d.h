// NCurses 2D vector math
// (c) 2022 by Stefan Roettger

#pragma once

// 2D vector type
struct Vec2
{
   float x;
   float y;
};

// 3D vector type
struct Vec3
{
   float x;
   float y;
   float z;
};

// 3D matrix type
struct Mat3
{
   float r1x, r1y, r1z;
   float r2x, r2y, r2z;
   float r3x, r3y, r3z;
};

// 2D vector construction
inline Vec2 vec2(float x = 0, float y = 0)
{
   Vec2 v = {x, y};
   return(v);
}

// 3D vector construction
inline Vec3 vec3(float x = 0, float y = 0, float z = 1)
{
   Vec3 v = {x, y, z};
   return(v);
}

// 3D matrix construction
inline Mat3 mat3(float r1x = 1, float r1y = 0, float r1z = 0,
                 float r2x = 0, float r2y = 1, float r2z = 0,
                 float r3x = 0, float r3y = 0, float r3z = 1)
{
   Mat3 M = {r1x, r1y, r1z, r2x, r2y, r2z, r3x, r3y, r3z};
   return(M);
}

// 2D vector addition
inline Vec2 add2(Vec2 a, Vec2 b)
{
   return(vec2(a.x+b.x, a.y+b.y));
}

// 2D vector subtraction
inline Vec2 sub2(Vec2 a, Vec2 b)
{
   return(vec2(a.x-b.x, a.y-b.y));
}

// 2D vector multiplication
inline Vec2 mul2(Vec2 a, Vec2 b)
{
   return(vec2(a.x*b.x, a.y*b.y));
}

// 2D vector multiplication with scalar value
inline Vec2 mul2s(Vec2 a, float s)
{
   return(vec2(a.x*s, a.y*s));
}

// 2D dot product
inline float dot2(Vec2 a, Vec2 b)
{
   return(a.x*b.x + a.y*b.y);
}

// 3D dot product
inline float dot3(Vec3 a, Vec3 b)
{
   return(a.x*b.x + a.y*b.y + a.z*b.z);
}

// get 3D matrix row
inline Vec3 row1(const Mat3 &m) {return(vec3(m.r1x, m.r1y, m.r1z));}
inline Vec3 row2(const Mat3 &m) {return(vec3(m.r2x, m.r2y, m.r2z));}
inline Vec3 row3(const Mat3 &m) {return(vec3(m.r3x, m.r3y, m.r3z));}

// get 3D matrix column
inline Vec3 col1(const Mat3 &m) {return(vec3(m.r1x, m.r2x, m.r3x));}
inline Vec3 col2(const Mat3 &m) {return(vec3(m.r1y, m.r2y, m.r3y));}
inline Vec3 col3(const Mat3 &m) {return(vec3(m.r1z, m.r2z, m.r3z));}

//! 3D matrix multiplication
inline Mat3 mul3(const Mat3 &m1, const Mat3 &m2)
{
   return(mat3(dot3(row1(m1), col1(m2)), dot3(row1(m1), col2(m2)), dot3(row1(m1), col3(m2)),
               dot3(row2(m1), col1(m2)), dot3(row2(m1), col2(m2)), dot3(row2(m1), col3(m2)),
               dot3(row3(m1), col1(m2)), dot3(row3(m1), col2(m2)), dot3(row3(m1), col3(m2))));
}

//! 3D matrix multiplication with right-hand side vector
inline Vec2 mul3v(const Mat3 &m, Vec3 v)
{
   Vec3 v3 = vec3(dot3(row1(m), v),
                  dot3(row2(m), v),
                  dot3(row3(m), v));

   float h = v3.z;
   if (h != 0)
      if (h != 1)
         h = 1/h;

   return(vec2(v3.x*h, v3.y*h));
}

//! get current transformation
Mat3 top();

//! push current transformation
void push();

//! pop current transformation
Mat3 pop();

//! apply translation to current transformation
void translate(float x, float y);

//! apply clockwise rotation to current transformation
void rotate(float a, float aspect = 2);

//! apply scaling to current transformation
void scale(float s, float t);
