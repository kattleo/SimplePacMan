// Polygon demo
// (c) 2022 by Stefan Roettger

#include "gfx.h"
#include "scrollarea.h"
#include "polygon.h"

int main(void)
{
   // init ASCII gfx
   init_gfx();
   init_color();

   // game area setup
   int sx = 52, sy = 28;
   set_area_size(sx, sy);
   set_window_size(COLS, LINES);
   translate(sx/2, sy/2);

   // vertex array
   Vec2 v[] = {0,-12, 7,-6, 3,-6, 13,0, 8,0, 17,6, // right tree top side
               -17,6, -8,0, -13,0, -3,-6, -7,-6, // left tree top side
               2,6, 2,12, -2,12, -2,6}; // tree trunk

   // interleaved vertex index array
   static const int n = 2; // number of polygons
   int p[3*n] = {11,0,3, 4,11,5}; // start index, run-length, and color index

   // game loop
   do
   {
      static int counter = 0;

      // clear game area
      clear_area();

      // render game area
      render_frame(0, 0, sx-1, sy-1);
      for (int i=0; i<n; i++)
         render_polygon(p[3*i], &v[p[3*i+1]], ACS_CKBOARD | COLOR_PAIR(p[3*i+2]));

      // update game area
      if (counter++ % 50 >= 20)
         rotate(3);

      // update log
      log_clear();
      log_text("frame counter:");
      log_text("%d", counter);
      log_text("");
      log_text("transformation matrix:");
      log_mat3(top());

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
