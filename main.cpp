
#include <stdio.h> 
#include <unistd.h> 
#include <ncurses.h> 
#include <stdlib.h>
#include "module.h"

int main()
{
   // init
   initscr();
   timeout(0);
   curs_set(FALSE);
   clear();
   game_loop();
   refresh();
   endwin();

   printf("time = %f\n", get_cur_time());

   return(0);
   
>>>>>>> 59b75e9 (Main/module etc.)
}