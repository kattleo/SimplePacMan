// NCurses side-scrolling area
// (c) 2020 by Stefan Roettger

#include "gridmenu.h"

#include "scrollarea.h"
#include "gridfont.h"

static int menux = 0, menuy = 0; // the size of the menu area
static int fontx = 0, fonty = 0; // the size of the grid font

static int *menu = NULL; // the menu area
static int selectx = 0, selecty = 0; // the selected menu cell
static int sprite = -1; // the sprite overlay

// set the size of the grid-font menu
void set_menu_size(int sx, int sy)
{
   if (sx < 1 || sy < 1) return;

   menux = sx;
   menuy = sy;

   fontx = get_grid_char_cols();
   fonty = get_grid_char_lines();

   if (menu) delete[] menu;
   menu = new int[sx*sy];

   clear_menu();
}

// clear the grid-font menu
void clear_menu(int ch)
{
   for (int i=0; i<menux; i++)
      for (int j=0; j<menuy; j++)
         set_menu(i, j, ch);
}

// set the menu cell at grid position (x, y) to character ch
void set_menu(int x, int y, int ch)
{
   if (!menu) return;

   if (x < 0) return;
   else if (x >= menux) return;

   if (y < 0) return;
   else if (y >= menuy) return;

   menu[x+y*menux] = ch;
}

// update the selected menu cell by moving the selection cursor by a position delta
void update_menu_selection(int dx, int dy)
{
   if (!menu) return;

   selectx += dx;
   selectx = selectx % menux;
   if (selectx < 0) selectx += menux;

   selecty += dy;
   selecty = selecty % menuy;
   if (selecty < 0) selecty += menuy;

   create_menu_overlay(-1, 0, 0);
}

// get the selected menu cell
int get_menu_selection()
{
   if (!menu) return(-1);

   return(menu[selectx+selecty*menux]);
}

// create a menu overlay
void create_menu_overlay(int num, int x, int y)
{
   if (!menu) return;

   if (sprite < 0)
   {
      sprite = num;
      enable_sprite(sprite, menux * (fontx + 1) + 1, menuy * (fonty + 1) + 1, true);
      set_sprite_position(sprite, x, y);
   }

   clear_sprite(sprite, ACS_CKBOARD);
   fill_sprite_area(sprite, selectx * (fontx + 1), selecty * (fonty + 1), fontx + 2, fonty + 2, ' '|COLOR_PAIR(9));

   for (int i=0; i<menux; i++)
      for (int j=0; j<menuy; j++)
      {
         int ch = menu[i+j*menux];
         print_sprite_grid_char(sprite, i * (fontx + 1) + 1, j * (fonty + 1) + 1, ch);
      }
}

// release allocated memory
void release_menu()
{
   if (sprite >= 0)
   {
      disable_sprite(sprite);
      sprite = -1;
   }

   if (menu) delete[] menu;
   menu = NULL;
}
