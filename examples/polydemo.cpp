// Polygon demo
// (c) 2022 by Stefan Roettger

#include "gfx.h"
#include "scrollarea.h"
#include "polygon.h"

int main(void)
{
   // init ASCII gfx
   init_gfx();

   // game area setup
   int sx = 52, sy = 28;
   set_area_size(sx, sy);
   set_window_size(COLS, LINES);
   translate(sx/2, sy/2);

   // define polygon vertices
   static const int n = 15;
   Vec2 v[n] = {0,-12,
                7,-6, 3,-6, 13,0, 8,0,
                17,6, 2,6, 2,12, -2,12, -2,6, -17,6,
                -8,0, -13,0, -3,-6, -7,-6};

   // game loop
   do
   {
      static int counter = 0;

      // clear game area
      clear_area();

      // render game area
      render_frame(0, 0, sx-1, sy-1);
      render_polygon(n, v);

      // update game area
      if (counter++ % 50 >= 20)
         rotate(3);

      // scroll game area
      center_window(sx/2, sy/2);

      // update log
      log_clear();
      log_text("frame counter:");
      log_text("%d", counter);
      log_text("");
      log_text("transformation matrix:");
      log_mat3(top());

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
