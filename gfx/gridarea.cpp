// NCurses side-scrolling grid-font area
// (c) 2020 by Stefan Roettger

#include "gridarea.h"

#include "scrollarea.h"
#include "gridfont.h"

static int gridx = 0, gridy = 0; // the size of the scrollable grid area
static int fontx = 0, fonty = 0; // the size of the grid font

static int *grid = NULL; // the scrollable grid area
static int *last = NULL; // the last scrollable grid area
static int *extra = NULL; // the scrollable extra grid area
static int *anim = NULL; // the last animated grid area

struct AnimType
{
   int *sequence;
   int count;
};

static const int ga_num = 256; // the size of the animation storage
static AnimType *ga_anims = NULL; // the animation storage

static bool wraparound = false; // the grid is wrapping around or not

void init_anims();
void release_anims();

// create a scrollable grid area
void set_grid_size(int sx, int sy)
{
   if (sx < 1 || sy < 1) return;

   gridx = sx;
   gridy = sy;

   fontx = get_grid_char_cols();
   fonty = get_grid_char_lines();

   set_area_size(gridx * fontx, gridy *fonty);

   if (grid) delete[] grid;
   grid = new int[sx*sy];

   if (last) delete[] last;
   last = new int[sx*sy];

   if (extra) delete[] extra;
   extra = new int[sx*sy];

   if (anim) delete[] anim;
   anim = new int[sx*sy];

   clear_grid();
   clear_extra();

   init_anims();
}

// get the width of the scrollable grid area
int get_grid_width()
{
   return(gridx);
}

// get the height of the scrollable grid area
int get_grid_height()
{
   return(gridy);
}

// set the size of the displayed grid window
void set_grid_window_size(int sx, int sy)
{
   if (sx < 1 || sy < 1) return;

   set_window_size(sx * fontx, sy * fonty);
}

// is the grid wrapping around or not?
void set_grid_wraparound(bool yes)
{
   wraparound = yes;
}

// clear the scrollable grid area
void clear_grid(int ch)
{
   int n = gridx * gridy;
   for (int i=0; i<n; i++)
   {
      last[i] = grid[i] = ch;
      anim[i] = -1;
   }
}

// clear the scrollable extra grid area
void clear_extra(int ch)
{
   int n = gridx * gridy;
   for (int i=0; i<n; i++)
      extra[i] = ch;
}

// set the border of the scrollable grid area
void set_grid_border(int ch)
{
   for (int i=0; i<gridx; i++)
   {
      set_grid(i, 0, ch);
      set_grid(i, gridy-1, ch);
   }

   for (int j=0; j<gridy; j++)
   {
      set_grid(0, j, ch);
      set_grid(gridx-1, j, ch);
   }
}

// helper for wrapping coordinates around
inline void wrap(int &x, int &y)
{
   if (wraparound)
   {
      if (x < 0) x += gridx;
      else if (x >= gridx) x -= gridx;

      if (y < 0) y += gridy;
      else if (y >= gridy) y -= gridy;
   }
}

// get the grid cell character at grid position (x, y)
int get_grid(int x, int y)
{
   if (!grid) return(-1);

   wrap(x, y);

   if (x < 0) return(-1);
   else if (x >= gridx) return(-1);

   if (y < 0) return(-1);
   else if (y >= gridy) return(-1);

   return(grid[x+y*gridx]);
}

// get the last cell character at grid position (x, y)
int get_last(int x, int y)
{
   if (!last) return(-1);

   wrap(x, y);

   if (x < 0) return(-1);
   else if (x >= gridx) return(-1);

   if (y < 0) return(-1);
   else if (y >= gridy) return(-1);

   return(last[x+y*gridx]);
}

// get the same cell character at grid position (x, y)
int get_same(int x, int y)
{
   if (!grid) return(-1);

   wrap(x, y);

   if (x < 0) return(-1);
   else if (x >= gridx) return(-1);

   if (y < 0) return(-1);
   else if (y >= gridy) return(-1);

   int ch = grid[x+y*gridx];
   if (ch != last[x+y*gridx]) return(-1);

   return(ch);
}

// get the extra grid cell character at grid position (x, y)
int get_extra(int x, int y)
{
   if (!extra) return(-1);

   wrap(x, y);

   if (x < 0) return(-1);
   else if (x >= gridx) return(-1);

   if (y < 0) return(-1);
   else if (y >= gridy) return(-1);

   return(extra[x+y*gridx]);
}

// get the last animated cell character at grid position (x, y)
int get_anim(int x, int y)
{
   return(anim[x+y*gridx]);
}

// set the grid cell at grid position (x, y) to character ch
void set_grid(int x, int y, int ch)
{
   if (!grid) return;
   if (ch < 0) return;

   wrap(x, y);

   if (x < 0) return;
   else if (x >= gridx) return;

   if (y < 0) return;
   else if (y >= gridy) return;

   grid[x+y*gridx] = ch;
}

// set the last cell at grid position (x, y) to character ch
void set_last(int x, int y, int ch)
{
   if (!last) return;
   if (ch < 0) return;

   wrap(x, y);

   if (x < 0) return;
   else if (x >= gridx) return;

   if (y < 0) return;
   else if (y >= gridy) return;

   last[x+y*gridx] = ch;
}

// set the same cell at grid position (x, y) to character ch
void set_same(int x, int y, int ch)
{
   if (!grid) return;
   if (ch < 0) return;

   wrap(x, y);

   if (x < 0) return;
   else if (x >= gridx) return;

   if (y < 0) return;
   else if (y >= gridy) return;

   grid[x+y*gridx] = ch;
   last[x+y*gridx] = ch;
}

// set the extra grid cell at grid position (x, y) to character ch
void set_extra(int x, int y, int ch)
{
   if (!extra) return;

   wrap(x, y);

   if (x < 0) return;
   else if (x >= gridx) return;

   if (y < 0) return;
   else if (y >= gridy) return;

   extra[x+y*gridx] = ch;
}

// set the last animated cell at grid position (x, y) to character ch
void set_anim(int x, int y, int ch)
{
   anim[x+y*gridx] = ch;
}

// check grid cell area for presence of character ch
bool check_grid(int ch)
{
   for (int j=0; j<gridy; j++)
      for (int i=0; i<gridx; i++)
         if (get_grid(i, j) == ch)
            return(true);

   return(false);
}

// count character ch in grid cell area
int count_grid(int ch)
{
   int count = 0;

   for (int j=0; j<gridy; j++)
      for (int i=0; i<gridx; i++)
         if (get_grid(i, j) == ch)
            count++;

   return(count);
}

// replace character ch in grid cell area
void replace_grid(int ch, int replace)
{
   for (int j=0; j<gridy; j++)
      for (int i=0; i<gridx; i++)
         if (get_grid(i, j) == ch)
            set_grid(i, j, replace);
}

// get the grid cell area at top-left position (x, y) with size (sx, sy)
int *get_grid_area(int x, int y,
                   int sx, int sy)
{
   if (sx < 1 || sy < 1) return(NULL);

   int *cells = new int[sx*sy];

   for (int j=0; j<sy; j++)
   {
      for (int i=0; i<sx; i++)
      {
         int ch = get_grid(i, j);
         cells[i+j*sx] = ch;
      }
   }

   return(cells);
}

// set a grid cell area at top-left position (x, y) with size (sx, sy)
void set_grid_area(int x, int y,
                   int sx, int sy,
                   const int *data)
{
   if (!data) return;

   for (int j=0; j<sy; j++)
   {
      for (int i=0; i<sx; i++)
      {
         int ch = data[i+j*sx];
         set_grid(x+i, y+j, ch);
      }
   }
}

// set the grid cells at top-left grid position (x, y) to a text string
void set_grid_text(int x, int y,
                   const char *text)
{
   int start = x;

   while (*text != '\0')
   {
      if (*text != '\n')
      {
         set_grid(x, y, *text);
         x++;
      }
      else
      {
         x = start;
         y++;
      }

      text++;
   }
}

// render a grid line from position (x1, y1) to (x2, y2)
void render_grid_line(int x1, int y1, int x2, int y2, int ch)
{
   int dx, dy;
   int ix, iy;

   // determine x increment
   if (x2 >= x1)
   {
      dx = x2-x1;
      ix = 1;
   }
   else
   {
      dx = x1-x2;
      ix = -1;
   }

   // determine y increment
   if (y2 >= y1)
   {
      dy = y2-y1;
      iy = 1;
   }
   else
   {
      dy = y1-y2;
      iy = -1;
   }

   int pdx, pdy;
   int ddx, ddy;
   int slow, fast;

   // determine fast direction
   if (dx > dy)
   {
      // x is fast direction
      pdx = ix; pdy = 0;
      ddx = ix; ddy = iy;
      slow = dy; fast = dx;
   }
   else
   {
      // y is fast direction
      pdx = 0; pdy = iy;
      ddx = ix; ddy = iy;
      slow = dx; fast = dy;
   }

   // loop along fast direction
   int x = x1, y = y1;
   int err = fast;
   int i = 0;
   while (TRUE)
   {
      set_grid(x, y, ch);

      if (i++ == fast)
         break;

      err -= slow<<1;

      if (err < 0)
      {
         err += fast<<1;
         x += ddx;
         y += ddy;
      }
      else
      {
         x += pdx;
         y += pdy;
      }
   }
}

// flood-fill a grid cell area starting at position (x, y)
void flood_fill_grid(int x, int y, int ch)
{
   int c = get_grid(x, y);
   if (c == ch) return;
   set_grid(x, y, ch);

   if (get_grid(x-1, y) == c) flood_fill_grid(x-1, y, ch);
   if (get_grid(x+1, y) == c) flood_fill_grid(x+1, y, ch);
   if (get_grid(x, y-1) == c) flood_fill_grid(x, y-1, ch);
   if (get_grid(x, y+1) == c) flood_fill_grid(x, y+1, ch);
}

// flood-fill everything but a grid cell area starting at position (x, y)
void inverse_flood_fill_grid(int x, int y, int ch)
{
   int c = get_grid(x, y);

   flood_fill_grid(x, y, 0);

   for (int j=0; j<gridx; j++)
   {
      for (int i=0; i<gridx; i++)
      {
         if (get_grid(i, j) == c)
            flood_fill_grid(i, j, ch);
      }
   }

   flood_fill_grid(x, y, c);
}

// place random characters
void place_random(int ch, int num, int exclude)
{
   for (int i=0; i<num; i++)
   {
      int x = rnd() * gridx;
      int y = rnd() * gridy;

      if (get_grid(x, y) != exclude)
         set_grid(x, y, ch);
   }
}

// update the displayed grid window
void update_grid_window()
{
   static int frame = 0;

   for (int i=0; i<gridx; i++)
      for (int j=0; j<gridy; j++)
      {
         int ch = get_grid(i, j);
         set_last(i, j, ch);

         if (ga_anims)
            if (ch >= 0 && ch < ga_num)
            {
               AnimType *anim = &ga_anims[ch];

               if (anim->sequence)
                  ch = anim->sequence[frame % anim->count];
            }

         if (ch != get_anim(i, j))
         {
            render_grid_char(i * fontx, j * fonty, ch);
            set_anim(i, j, ch);
         }
      }

   frame++;
}

// redraw the displayed grid window at center grid position (x, y)
void redraw_grid_window(int x, int y)
{
   update_grid_window();

   int wx = get_window_width();
   int wy = get_window_height();

   int px = x * fontx + fontx/2 - wx/2;
   int py = y * fonty + fonty/2 - wy/2;

   redraw_window(px, py);
}

// scroll the displayed grid window to center grid position (x, y)
void scroll_grid_window(int x, int y, int deltax, int deltay, bool stop)
{
   update_grid_window();

   int wx = get_window_width();
   int wy = get_window_height();

   int px = x * fontx + fontx/2 - wx/2;
   int py = y * fonty + fonty/2 - wy/2;

   scroll_window(px, py, deltax, deltay, stop);
}

// release allocated memory
void release_grid()
{
   release_area();

   if (grid) delete[] grid;
   grid = NULL;

   if (last) delete[] last;
   last = NULL;

   if (extra) delete[] extra;
   extra = NULL;

   if (anim) delete[] anim;
   anim = NULL;

   release_anims();
}

// initialize animation storage
void init_anims()
{
   release_anims();

   ga_anims = new AnimType[ga_num];

   AnimType anim = {NULL, 0};
   for (int i=0; i<ga_num; i++)
      ga_anims[i] = anim;
}

// set character animation
void set_grid_animation(int ch, int num, const int *data)
{
   if (ch < 0 || ch > ga_num) return;
   if (num <= 0) return;

   if (ga_anims)
   {
      if (ga_anims[ch].sequence)
         delete[] ga_anims[ch].sequence;

      ga_anims[ch].sequence = new int[num];
      for (int i=0; i<num; i++)
         ga_anims[ch].sequence[i] = data[i];

      ga_anims[ch].count = num;
   }
}

// set character animation string
void set_grid_animation_string(int ch, const char *str)
{
   if (ch < 0 || ch > ga_num) return;
   int num = strlen(str);

   if (ga_anims)
   {
      if (ga_anims[ch].sequence)
         delete[] ga_anims[ch].sequence;

      ga_anims[ch].sequence = new int[num];
      for (int i=0; i<num; i++)
         ga_anims[ch].sequence[i] = str[i];

      ga_anims[ch].count = num;
   }
}

// release animation storage
void release_anims()
{
   if (ga_anims)
   {
      for (int i=0; i<ga_num; i++)
         if (ga_anims[i].sequence)
            delete[] ga_anims[i].sequence;

      delete[] ga_anims;
      ga_anims = NULL;
   }
}
