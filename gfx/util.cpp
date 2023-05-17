// NCurses utility functions
// (c) 2020 by Stefan Roettger

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "gfx.h"
#include "scrollarea.h"

// sleep for a period of time given in milli seconds
void msleep(float ms)
{
   int us = ms*1000; // micro secs
   usleep(us);
}

// generate a random float number in the range [0,1[
float rnd()
{
   static bool init = false;

   if (!init)
   {
      srand((int)time(NULL));
      init = true;
   }

   return(rand()/((float)RAND_MAX+1));
}

// check for a repeating time period
bool is_repeating(float time, float period,
                  float percent)
{
   float w = time/period-(int)(time/period);
   return(w<percent/100);
}

// check if a string starts with a prefix
int strpre(const char *pre, const char *str)
{
   return(strncmp(pre, str, strlen(pre)));
}

// retrieve the n-th argument from the command line arguments
const char *get_arg(int n, int argc, char *argv[])
{
   int count = 0;

   for (int i=1; i<argc; i++)
      if (strpre("-", argv[i]) != 0)
      {
         count++;
         if (count == n)
            return(argv[i]);
      }

   return(NULL);
}

// retrieve the n-th option from the command line arguments
const char *get_opt(int n, int argc, char *argv[],
                    double *value, double default_value)
{
   int count = 0;

   for (int i=1; i<argc; i++)
      if (strpre("-", argv[i]) == 0)
      {
         count++;
         if (count == n)
         {
            const char *opt = argv[i]+1;
            if (strpre("--", argv[i]) == 0) opt = argv[i]+2;

            if (value)
            {
               *value = default_value;
               const char *del = strchr(opt, '=');

               if (del)
                  sscanf(del+1, "%lf", value);
            }

            return(opt);
         }
      }

   return(NULL);
}

static int log_line = 0; // the actual line of the log text

// print log text in overlay
void log_text(const char *format, ...)
{
   if (has_window())
   {
      int s = get_sprite_num()-1;

      if (!is_sprite_enabled(s))
      {
         int winx = get_window_width();
         int winy = get_window_height();

         enable_sprite(s, winx-2, winy-2, true);
         set_sprite_position(s, 1, 1);
      }

      int height = get_sprite_height(s);

      va_list arg;
      va_start(arg, format);
      int n = vsnprintf(NULL, 0, format, arg);
      va_end(arg);

      char *text = get_string_buffer(n);
      va_start(arg, format);
      vsnprintf(text, n+1, format, arg);
      va_end(arg);

      int lines = print_sprite_text(s, 0, log_line, text);
      log_line += lines;

      while (log_line > height)
      {
         scroll_sprite_up(s);
         log_line--;
         print_sprite_text(s, 0, log_line-lines-1, text);
      }
   }
}

// print 2D vector to log overlay
void log_vec2(const Vec2 &v)
{
   log_text("( % 10.6f % 10.6f )", v.x, v.y);
}

// print 3D vector to log overlay
void log_vec3(const Vec3 &v)
{
   log_text("( % 10.6f % 10.6f % 10.6f )", v.x, v.y, v.z);
}

// print 3x3 matrix to log overlay
void log_mat3(const Mat3 &m)
{
   log_text("/ % 10.6f % 10.6f % 10.6f \\\n"
            "| % 10.6f % 10.6f % 10.6f |\n"
            "\\ % 10.6f % 10.6f % 10.6f /",
            m.r1x, m.r1y, m.r1z,
            m.r2x, m.r2y, m.r2z,
            m.r3x, m.r3y, m.r3z);
}

// clear log overlay
void log_clear()
{
   if (has_window())
   {
      int s = get_sprite_num()-1;
      disable_sprite(s);
      log_line = 0;
   }
}
