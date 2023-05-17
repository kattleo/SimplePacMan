// NCurses grid-font
// (c) 2020 by Stefan Roettger

#include "gridfont.h"

#include <ctype.h>
#include <ncurses.h>

static int gc_cols = 5;
static int gc_lines = 3;

static const int gc_num = 256;

static int **font = NULL;

void set_default_grid_chars();

// initialize grid font with character size (cols, lines)
void init_grid_font(int cols, int lines)
{
   if (cols > 0 && lines > 0)
   {
      gc_cols = cols;
      gc_lines = lines;
   }

   if (font) delete font;
   font = new int *[gc_num];

   for (int i=0; i<gc_num; i++)
   {
      font[i] = new int[gc_cols*gc_lines];

      for (int j=0; j<gc_cols*gc_lines; j++)
         font[i][j] = ' ';
   }

   set_default_grid_chars();
}

// get character cols
int get_grid_char_cols()
{
   return(gc_cols);
}

// get character lines
int get_grid_char_lines()
{
   return(gc_lines);
}

// set character by data array
void set_grid_char_data(int ch, const int *data)
{
   if (!font) return;
   if (ch < 0 || ch >= gc_num) return;

   for (int i=0; i<gc_cols*gc_lines; i++)
      font[ch][i] = data[i];
}

// set character by text string
void set_grid_char_text(int ch, const char *text, bool interprete)
{
   if (!font) return;
   if (ch < 0 || ch >= gc_num) return;

   int count = 0;
   int attr = 0;

   while (*text != '\0')
   {
      int c = *text++;

      if (interprete)
         switch (c)
         {
            case '^': c = ACS_S1; break;
            case 'B': attr |= A_BOLD; continue;
            case '1': attr |= COLOR_PAIR(1); continue;
            case '2': attr |= COLOR_PAIR(2); continue;
            case '3': attr |= COLOR_PAIR(3); continue;
            case '4': attr |= COLOR_PAIR(4); continue;
            case '5': attr |= COLOR_PAIR(5); continue;
            case '6': attr |= COLOR_PAIR(6); continue;
            case '7': attr |= COLOR_PAIR(7); continue;
            case '8': attr |= COLOR_PAIR(8); continue;
            case '9': attr |= COLOR_PAIR(9); continue;
            case '0': attr = 0; continue;
         }

      font[ch][count++] = c | attr;

      if (count == gc_cols * gc_lines)
         break;
   }
}

// get character data
int *get_grid_char_data(int ch)
{
   if (!font) return(NULL);
   if (ch < 0 || ch >= gc_num) return(NULL);

   return(font[ch]);
}

// release allocated memory
void release_grid_font()
{
   if (font)
   {
      for (int i=0; i<gc_num; i++)
         delete font[i];

      delete font;
      font = NULL;
   }
}

// set default alphabet
void set_default_grid_chars()
{
   if (gc_cols != 5 || gc_lines != 3) return;

   // 5x3 letters
   set_grid_char_text('A',
                      "/^^\\ "
                      "|--| "
                      "|  | ", true);
   set_grid_char_text('B',
                      "|^^\\ "
                      "|--| "
                      "|__/ ", true);
   set_grid_char_text('C',
                      "/^^\\ "
                      "|    "
                      "\\__/ ", true);
   set_grid_char_text('D',
                      "|^^\\ "
                      "|  | "
                      "|__/ ", true);
   set_grid_char_text('E',
                      "|^^^ "
                      "|--  "
                      "|___ ", true);
   set_grid_char_text('F',
                      "|^^^ "
                      "|--  "
                      "|    ", true);
   set_grid_char_text('G',
                      "/^^  "
                      "| ^\\ "
                      "\\__/ ", true);
   set_grid_char_text('H',
                      "|  | "
                      "|--| "
                      "|  | ", true);
   set_grid_char_text('I',
                      "  |  "
                      "  |  "
                      "  |  ", true);
   set_grid_char_text('J',
                      "   | "
                      "   | "
                      "\\__/ ", true);
   set_grid_char_text('K',
                      "|  / "
                      "|-|  "
                      "|  \\ ", true);
   set_grid_char_text('L',
                      "|    "
                      "|    "
                      "|___ ", true);
   set_grid_char_text('M',
                      "|\\/| "
                      "|  | "
                      "|  | ", true);
   set_grid_char_text('N',
                      "|\\ | "
                      "| \\| "
                      "|  | ", true);
   set_grid_char_text('O',
                      "/^^\\ "
                      "|  | "
                      "\\__/ ", true);
   set_grid_char_text('P',
                      "|^^| "
                      "|__| "
                      "|    ", true);
   set_grid_char_text('Q',
                      "/^^\\ "
                      "|  | "
                      "\\__X ", true);
   set_grid_char_text('R',
                      "|^^| "
                      "|__| "
                      "|  \\ ", true);
   set_grid_char_text('S',
                      "|^^^ "
                      "\\--\\ "
                      "___| ", true);
   set_grid_char_text('T',
                      "^^|^^"
                      "  |  "
                      "  |  ", true);
   set_grid_char_text('U',
                      "|  | "
                      "|  | "
                      "\\__/ ", true);
   set_grid_char_text('V',
                      "\\   /"
                      " \\ / "
                      "  V  ", true);
   set_grid_char_text('W',
                      "|  | "
                      "|  | "
                      "\\/\\/ ", true);
   set_grid_char_text('X',
                      "\\ /  "
                      " X   "
                      "/ \\  ", true);
   set_grid_char_text('Y',
                      "\\ /  "
                      " V   "
                      " |   ", true);
   set_grid_char_text('Z',
                      "^^^^/"
                      "  /  "
                      "/____", true);

   // 5x3 digits
   set_grid_char_text('0',
                      "|^^| "
                      "| /| "
                      "|/_| ", true);
   set_grid_char_text('1',
                      " /|  "
                      "/ |  "
                      "  |  ", true);
   set_grid_char_text('2',
                      "/^^\\ "
                      " __/ "
                      "/___ ", true);
   set_grid_char_text('3',
                      "/^^\\ "
                      " --| "
                      "\\__/ ", true);
   set_grid_char_text('4',
                      "|  | "
                      "|__| "
                      "   | ", true);
   set_grid_char_text('5',
                      "|^^^ "
                      "|--\\ "
                      "___/ ", true);
   set_grid_char_text('6',
                      "|^^  "
                      "|^^| "
                      "|__| ", true);
   set_grid_char_text('7',
                      "^^^| "
                      "  /  "
                      " /   ", true);
   set_grid_char_text('8',
                      "|^^| "
                      "|--| "
                      "|__| ", true);
   set_grid_char_text('9',
                      "|^^| "
                      "|__| "
                      " __| ", true);

   // 5x3 special chars
   set_grid_char_text('.',
                      "     "
                      "     "
                      " o   ", true);
   set_grid_char_text(',',
                      "     "
                      "     "
                      " /   ", true);
   set_grid_char_text(':',
                      "     "
                      " o   "
                      " o   ", true);
   set_grid_char_text(';',
                      "     "
                      " o   "
                      " /   ", true);
   set_grid_char_text('-',
                      "     "
                      " --- "
                      "     ", true);
   set_grid_char_text('+',
                      "  .  "
                      " -+- "
                      "  '  ", true);
   set_grid_char_text('*',
                      " . . "
                      "  x  "
                      " ' ' ", true);
   set_grid_char_text('/',
                      "   / "
                      "  /  "
                      " /   ", true);
   set_grid_char_text('\\',
                      " \\   "
                      "  \\  "
                      "   \\ ", true);
   set_grid_char_text('<',
                      "     "
                      "  /  "
                      "  \\  ", true);
   set_grid_char_text('>',
                      "     "
                      "  \\  "
                      "  /  ", true);
   set_grid_char_text('=',
                      " ___ "
                      " ___ "
                      "     ", true);
   set_grid_char_text('(',
                      "  /  "
                      "  |  "
                      "  \\  ", true);
   set_grid_char_text(')',
                      "  \\  "
                      "  |  "
                      "  /  ", true);
   set_grid_char_text('[',
                      "  |^ "
                      "  |  "
                      "  |_ ", true);
   set_grid_char_text(']',
                      " ^|  "
                      "  |  "
                      " _|  ", true);
   set_grid_char_text('{',
                      "  /^ "
                      " <   "
                      "  \\_ ", true);
   set_grid_char_text('}',
                      " ^\\  "
                      "   > "
                      " _/  ", true);
   set_grid_char_text('|',
                      "  |  "
                      "  |  "
                      "  |  ", true);
   set_grid_char_text('_',
                      "     "
                      "     "
                      "____ ", true);
   set_grid_char_text('\'',
                      "  |  "
                      "     "
                      "     ", true);
   set_grid_char_text('"',
                      " | | "
                      "     "
                      "     ", true);
   set_grid_char_text('!',
                      "  |  "
                      "  |  "
                      "  .  ", true);
   set_grid_char_text('?',
                      "/^^| "
                      "  /  "
                      "  !  ", true);
   set_grid_char_text('@',
                      "/^^\\ "
                      "|(]| "
                      "\\__  ", true);
   set_grid_char_text('#',
                      "+--+ "
                      "|  | "
                      "+--+ ", true);
   set_grid_char_text('$',
                      " /|^ "
                      " \\|\\ "
                      " _|/ ", true);
   set_grid_char_text('%',
                      " O / "
                      "  /  "
                      " / O ", true);
   set_grid_char_text('&',
                      "( ) "
                      " X ,"
                      "(_X_", true);
   set_grid_char_text('^',
                      " /\\  "
                      "     "
                      "     ", true);
   set_grid_char_text('~',
                      " _  "
                      "/ \\_/"
                      "     ", true);

   // lower case copy
   for (int c='a'; c<='z'; c++)
      set_grid_char_data(c, get_grid_char_data(toupper(c)));
}
