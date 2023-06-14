#include <ncurses.h>

int main()
{
   initscr();
   curs_set(FALSE);
   clear();

   move(LINES/2, COLS/2 - 7);
   printw("hello, ncurses!");
   refresh();

   getch();
   endwin();

   return(0);
}
