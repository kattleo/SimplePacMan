#include <ncurses.h>

int main()
{
   initscr();
   clear();

   move(10, 10);
   printw("hello, ncurses!");
   refresh();

   getch();
   endwin();

   return(0);
}
