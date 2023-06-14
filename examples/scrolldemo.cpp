// Scroll demo
// (c) 2020 by Stefan Roettger

#include "gfx.h"
#include "scrollarea.h"

void render_fract_triangle(int width, int height,
                           long itermax = 10000);

int main(void)
{
   // init ASCII gfx
   init_gfx();
   init_color();

   // game area setup
   int sx = 60, sy = 30;
   set_area_size(sx, sy);
   set_window_size(COLS, LINES);

   // render game area
   render_frame(0, 0, sx-1, sy-1);
   render_text_format(2, sy-3, "scrollable area:\ncols=%d lines=%d", sx, sy);
   log_text("window area:\ncols=%d lines=%d", COLS, LINES);
   set_cell_offset(6, 3);
   render_fract_triangle(sx-12, sy-6, 10000);
   set_cell_offset();

   // position window area
   position_window(sx/2, -sy/2);

   // game loop
   do
   {
      // scroll game area
      center_window(sx/2, sy/2);

      // refresh screen
      refresh();

      // wait until next frame
      msleep(100); // 100 ms = 10 fps
   }
   while (tolower(getch()) != 'q');

   // release allocated memory
   release_area();

   // exit ASCII gfx
   exit_gfx();

   return(0);
}

void render_fract_triangle(int width, int height,
                           long itermax)
{
   long iter;
   int maxcols, maxlines;
   int xi, yi;
   int x[3], y[3];
   int index;

   /* initialize the ifs vectors */

   maxcols = width - 1;
   maxlines = height - 1;

   x[0] = 0;
   y[0] = 0;

   x[1] = maxcols / 2;
   y[1] = maxlines;

   x[2] = maxcols;
   y[2] = 0;

   /* iterate the ifs system */

   xi = rand() % maxcols;
   yi = rand() % maxlines;

   for (iter = 0; iter < itermax; iter++)
   {
      index = rand() % 3;

      xi = (xi + x[index]) / 2;
      yi = (yi + y[index]) / 2;

      set_cell(xi, yi, '*' | COLOR_PAIR(index+2));
   }
}
