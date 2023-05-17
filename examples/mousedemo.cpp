// Mouse demo
// (c) 2023 by Stefan Roettger

#include "gfx.h"
#include "mouse.h"

int main(void)
{
   // init ASCII gfx
   init_gfx();

   // init mouse event reporting
   mouse_init();

   // game loop
   bool run = true;
   do
   {
      // process keycodes
      int c;
      while ((c = get_keycode()) != ERR)
      {
         // decode keycodes
         unsigned int x, y, b;
         if (mouse_keycode(c, &x, &y, &b))
         {
            // show mouse events
            mvprintw((LINES-1)/2, 1, "mouse event: x=%d y=%d b=%d", x, y, b);
         }
         else
         {
            // show key events
            mvprintw((LINES-1)/2, 1, "key event: 0x%02x", c);

            // check for exit
            if (tolower(c) == 'q')
               run = false;
         }

         // clear to end of line
         clrtoeol();
         refresh();
      }

      // wait until next frame
      msleep(20); // 20 ms = 50 fps
   }
   while (run);

   // exit mouse event reporting
   mouse_exit();

   // exit ASCII gfx
   exit_gfx();

   return(0);
}
