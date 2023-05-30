// ASCII GFX example
// (c) 2020 by Stefan Roettger

#include "gfx.h"
#include "sound.h"
#include "gridfont.h"
#include "../game.h"


int main(int argc, char *argv[])
{
   // init ASCII gfx
   init_gfx();
   init_color();

   // init sound
   sound_init();


   // refresh screen
   refresh();

   game_loop();

   

   sound_exit();

   // exit ASCII gfx
   exit_gfx();

   return(0);
}
