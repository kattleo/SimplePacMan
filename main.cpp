#include <ncurses.h>
#include <unistd.h>

void myflash()
{
   int i;
   const int n = 5;

   for (i=0; i<n; i++)
   {
      flash();
      refresh();
      clear();
      refresh();
      sleep(1);
   }
}

int main()
{
    initscr();
    clear();
    flash();
    refresh();
    myflash();
    endwin();
    return(0);
}