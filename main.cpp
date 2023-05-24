// ASCII GFX example
// (c) 2020 by Stefan Roettger

#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "game.h"
#include "gfx/util.h"

static int max_x = 0, max_y = 0; // screen size
double time;
int main(int argc, char *argv[])
{
   // init ASCII gfx
   init_gfx();
   init_color();

   // init sound
   sound_init();

   // print screen size
   // * LINES and COLS are predefined macros
   move(0, 0); // position cursor at origin
   printw("screen size is %dx%d\n", COLS, LINES);
   hline(ACS_HLINE, COLS);

   
   // * a preceding & as used by the call-by-ref convention is not required
   // * since getmaxyx is a preprocessor macro
   getmaxyx(stdscr, max_y, max_x); // & not required

  

   // draw horizontal line
   mvhline(max_y-3, 0, ACS_HLINE, max_x);

   // terminal bell
   beep();

   // play WAV sound
   
   game_loop();


   // refresh screen
   refresh();

   // exit sound
   sound_exit();

   // exit ASCII gfx
   exit_gfx();

   return(0);
}
