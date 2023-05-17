// Joystick demo
// (c) 2023 by Stefan Roettger

#include "gfx.h"
#include "joystick.h"

int main(void)
{
   // init ASCII gfx
   init_gfx();

   // init joystick
   joystick_init();

   // game loop
   bool run = true;
   do
   {
      // get keycodes
      int c = getch();

      // decode joystick state
      int dx, dy, buttons;
      joystick_state(&dx, &dy, &buttons);

      if (joystick_detected())
      {
         // show joystick state
         mvprintw((LINES-1)/2, 1, "joystick state: dx=%d dy=%d b=%d", dx, dy, buttons);
      }
      else
      {
         // show key events
         if (c != ERR)
            mvprintw((LINES-1)/2+1, 1, "key event: 0x%02x", c);

         // show joystick info
         mvprintw((LINES-1)/2, 1, "no joystick detected");
      }

      // check for exit
      if (tolower(c) == 'q')
         run = false;

      // clear to end of line
      clrtoeol();
      refresh();

      // wait until next frame
      msleep(20); // 20 ms = 50 fps
   }
   while (run);

   // exit joystick
   joystick_exit();

   // exit ASCII gfx
   exit_gfx();

   return(0);
}
