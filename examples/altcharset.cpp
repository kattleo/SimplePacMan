#include <ncurses.h>

int main()
{
   initscr();

   // heading
   clear();
   attron(A_UNDERLINE);
   printw("NCURSES ALTCHARSET CHARACTERS\n\n");
   attroff(A_UNDERLINE);

   // loop over character range
   int counter, counter2=0;
   for (counter=32; counter<128; counter++)
   {
      // output one altcharset character
      printw("%3d = ", counter);
      addch(counter | A_ALTCHARSET);

      // output table dividers
      if (counter2 < 7)
      {
         addch(' ');
         addch(ACS_VLINE);
         addch(' ');
      }

      // wrap table columns
      if (++counter2 > 7)
      {
         addch('\n');
         counter2 = 0;
      }

      // skip unprintable characters
      if (counter == 127)
         counter = 159;
   }

   // wait for keypress
   printw("\npress any key\n");
   refresh();
   getch();

   // heading
   clear();
   attron(A_UNDERLINE);
   printw("NCURSES EXTENDED CHARACTERS\n\n");
   attroff(A_UNDERLINE);

   // print symbolic character names
   printw("Upper left corner         "); addch(ACS_ULCORNER); printw("  ACS_ULCORNER\t");
   printw("Lower left corner         "); addch(ACS_LLCORNER); printw("  ACS_LLCORNER\n");
   printw("Upper right corner        "); addch(ACS_URCORNER); printw("  ACS_URCORNER\t");
   printw("Lower right corner        "); addch(ACS_LRCORNER); printw("  ACS_LRCORNER\n");
   printw("Tee pointing right        "); addch(ACS_LTEE); printw("  ACS_LTEE\t\t");
   printw("Tee pointing left         "); addch(ACS_RTEE); printw("  ACS_RTEE\n");
   printw("Tee pointing up           "); addch(ACS_BTEE); printw("  ACS_BTEE\t\t");
   printw("Tee pointing down         "); addch(ACS_TTEE); printw("  ACS_TTEE\n");
   printw("Horizontal line           "); addch(ACS_HLINE); printw("  ACS_HLINE\t\t");
   printw("Vertical line             "); addch(ACS_VLINE); printw("  ACS_VLINE\n");
   printw("Large Plus or cross over  "); addch(ACS_PLUS); printw("  ACS_PLUS\t\t");
   printw("Scan Line 1               "); addch(ACS_S1); printw("  ACS_S1\n");
   printw("Scan Line 3               "); addch(ACS_S3); printw("  ACS_S3\t\t");
   printw("Scan Line 7               "); addch(ACS_S7); printw("  ACS_S7\n");
   printw("Scan Line 9               "); addch(ACS_S9); printw("  ACS_S9\t\t");
   printw("Diamond                   "); addch(ACS_DIAMOND); printw("  ACS_DIAMOND\n");
   printw("Checker board (stipple)   "); addch(ACS_CKBOARD); printw("  ACS_CKBOARD\t");
   printw("Degree Symbol             "); addch(ACS_DEGREE); printw("  ACS_DEGREE\n");
   printw("Plus/Minus Symbol         "); addch(ACS_PLMINUS); printw("  ACS_PLMINUS\t");
   printw("Bullet                    "); addch(ACS_BULLET); printw("  ACS_BULLET\n");
   printw("Arrow Pointing Left       "); addch(ACS_LARROW); printw("  ACS_LARROW\t\t");
   printw("Arrow Pointing Right      "); addch(ACS_RARROW); printw("  ACS_RARROW\n");
   printw("Arrow Pointing Down       "); addch(ACS_DARROW); printw("  ACS_DARROW\t\t");
   printw("Arrow Pointing Up         "); addch(ACS_UARROW); printw("  ACS_UARROW\n");
   printw("Board of squares          "); addch(ACS_BOARD); printw("  ACS_BOARD\t\t");
   printw("Lantern Symbol            "); addch(ACS_LANTERN); printw("  ACS_LANTERN\n");
   printw("Solid Square Block        "); addch(ACS_BLOCK); printw("  ACS_BLOCK\t\t");
   printw("Less/Equal sign           "); addch(ACS_LEQUAL); printw("  ACS_LEQUAL\n");
   printw("Greater/Equal sign        "); addch(ACS_GEQUAL); printw("  ACS_GEQUAL\t\t");
   printw("Pi                        "); addch(ACS_PI); printw("  ACS_PI\n");
   printw("Not equal                 "); addch(ACS_NEQUAL); printw("  ACS_NEQUAL\t\t");
   printw("UK pound sign             "); addch(ACS_STERLING); printw("  ACS_STERLING\n");

   // wait for keypress
   printw("\npress any key\n");
   refresh();
   getch();

   endwin();

   return(0);
}
