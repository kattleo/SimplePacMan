// mouse input for NCurses
// (c) 2023 by Stefan Roettger

#include "mouse.h"

// init mouse reporting
void mouse_init()
{
   // report all mouse events
   mousemask(ALL_MOUSE_EVENTS, NULL);
}

// decode wide keycode
bool mouse_keycode(int keycode,
                   unsigned int *x, unsigned int *y,
                   unsigned int *buttons)
{
   if (keycode == KEY_MOUSE)
   {
      MEVENT event;
      if (getmouse(&event) == OK)
      {
         *x = event.x;
         *y = event.y;
         *buttons = event.bstate & 0xff;
         return(true);
      }
   }

   return(false);
}

// exit mouse reporting
void mouse_exit()
{
   // suppress all mouse events
   mousemask(0, NULL);
}
