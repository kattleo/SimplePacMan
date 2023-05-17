// NCurses 2D vector math
// (c) 2022 by Stefan Roettger

#include <math.h>

#include "math2d.h"

static const int mat3_stack_max = 100;
static int mat3_stack_size = 0;
static Mat3 mat3_stack[mat3_stack_max];

// get current transformation
Mat3 top()
{
   if (mat3_stack_size > 0)
      return(mat3_stack[mat3_stack_size-1]);
   else
      return(mat3());
}

// push current transformation
void push()
{
   if (mat3_stack_size < mat3_stack_max)
   {
      if (mat3_stack_size > 0)
         mat3_stack[mat3_stack_size] = mat3_stack[mat3_stack_size-1];
      else
         mat3_stack[mat3_stack_size] = mat3();
      mat3_stack_size++;
   }
}

// pop current transformation
Mat3 pop()
{
   if (mat3_stack_size > 0)
      return(mat3_stack[--mat3_stack_size]);
   else
      return(mat3());
}

// apply translation to current transformation
void translate(float x, float y)
{
   if (mat3_stack_size > 0)
   {
      Mat3 m = mat3(1,0,x, 0,1,y, 0,0,1);
      mat3_stack[mat3_stack_size-1] = mul3(mat3_stack[mat3_stack_size-1], m);
   }
}

// apply clockwise rotation to current transformation
void rotate(float a, float aspect)
{
   if (mat3_stack_size > 0)
   {
      float w = M_PI*a/180;
      float s = sin(w);
      float c = cos(w);

      Mat3 m = mat3(c,-s*aspect,0, s/aspect,c,0, 0,0,1);
      mat3_stack[mat3_stack_size-1] = mul3(mat3_stack[mat3_stack_size-1], m);
   }
}

// apply scaling to current transformation
void scale(float s, float t)
{
   if (mat3_stack_size > 0)
   {
      Mat3 m = mat3(s,0,0, 0,t,0, 0,0,1);
      mat3_stack[mat3_stack_size-1] = mul3(mat3_stack[mat3_stack_size-1], m);
   }
}
