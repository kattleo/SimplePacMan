// NCurses graphics
// (c) 2020 by Stefan Roettger

#include "gfx.h"
#include "math2d.h"
#include "gridfont.h"

static WINDOW *W = NULL; // the drawing window

static char *string_buffer = NULL; // the string buffer
static int buffer_size = 0; // the string buffer size

// init ASCII GFX
void init_gfx()
{
   // initialize ncurses screen
   initscr(); // initialize the screen to contain a single window
   curs_set(FALSE); // disable text cursor
   timeout(0); // configure getch() to be non-blocking
   keypad(stdscr, TRUE); // configure getch() to return cursor key codes
   noecho(); // do not echo keyboard input
   push(); // init matrix stack
}

// init ASCII GFX color display
void init_color()
{
   start_color(); // enable color display
   init_pair(1, COLOR_WHITE, COLOR_BLACK); // 1st fore-/background color pair with index #1
   init_pair(2, COLOR_RED, COLOR_BLACK);   // 2nd fore-/background color pair with index #2
   init_pair(3, COLOR_GREEN, COLOR_BLACK); // 3rd fore-/background color pair with index #3
   init_pair(4, COLOR_BLUE, COLOR_BLACK);  // 4th fore-/background color pair with index #4
   bkgd(COLOR_PAIR(1));
}

// exit ASCII GFX
void exit_gfx()
{
   // restore normal terminal behaviour before exiting
   endwin();

   // deallocate string buffer
   delete string_buffer;
   string_buffer = NULL;
   buffer_size = 0;
}

// set the drawing window
void set_window(WINDOW *w)
{
   W = w;
}

// use indexed color pair for foreground/background colors of characters
void use_color(int index)
{
   color_set(index, NULL);
}

//! use attribute bold
void use_attr_bold()
{
   attron(A_BOLD);
}

//! use attribute blink
void use_attr_blink()
{
   attron(A_BLINK);
}

//! use attribute reverse
void use_attr_reverse()
{
   attron(A_REVERSE);
}

//! use attribute normal
void use_attr_normal()
{
   attrset(A_NORMAL);
}

// draw a formatted text at position (x, y)
void draw_text(int y, int x,
               const char *format, ...)
{
   va_list argp;
   va_start(argp, format);
   int n = vsnprintf(NULL, 0, format, argp);
   va_end(argp);

   char *text = get_string_buffer(n);
   va_start(argp, format);
   vsnprintf(text, n+1, format, argp);
   va_end(argp);

   move(y, x);
   int c = strlen(text);
   for (int i=0; i<c; i++)
      addch(isprint(text[i])?text[i]:' ');
}

// draw a sprite at barycenter position (x, y)
void draw_sprite(int y, int x,
                 const char *text)
{
   WINDOW *w = W?W:stdscr;

   // calculate sprite size
   int cols = 0;
   int rows = 1;
   const char *c = text;
   while (*c != '\0')
   {
      if (*c++ != '\n')
         cols++;
      else
         rows++;
   }

   // offset from barycenter by subtracting half sprite size
   cols /= rows;
   x -= cols/2;
   y -= rows/2;

   // print consecutive sprite lines
   wmove(w, y, x);
   while (*text != '\0')
   {
      if (*text != '\n')
         waddch(w, *text);
      else
         wmove(w, ++y, x);

      text++;
   }
}

// draw a screen area at top-left position (x, y) with size (sx, sy)
void draw_area(int y, int x,
               int sy, int sx,
               const int *data)
{
   WINDOW *w = W?W:stdscr;

   for (int j=0; j<sy; j++)
   {
      wmove(w, y + j, x);
      for (int i=0; i<sx; i++)
      {
         int ch = data[i+j*sx];
         if (ch >= 0) waddch(w, ch);
      }
   }
}

// draw a text string with grid font characters at top-left position (x, y)
void draw_grid_text(int y, int x,
                    const char *text)
{
   int start = x;

   int sy = get_grid_char_lines();
   int sx = get_grid_char_cols();

   while (*text != '\0')
   {
      if (*text != '\n')
      {
         int *data = get_grid_char_data(*text);
         draw_area(y, x, sy, sx, data);
         x += sx;
      }
      else
      {
         x = start;
         y += sy;
      }

      text++;
   }
}

/*

 Evolution from a naive line drawing implementation, which draws a line
 from position (x1, y1) to (x2, y2), to the Bresenham algorithm:

 Step 1) Naive implementation (horizontal case)

 float dx = x2 - x1;
 float dy = y2 - y1;
 float delta = dy / dx;
 int x = x1;
 float y = y1;
 while (x++ <= x2)
 {
    mvaddch((int)(y+0.5), x, '*');
    y = y + delta;
 }

 Step 2) Differential implementation (float version)

 float dx = x2 - x1;
 float dy = y2 - y1;
 float delta = dy / dx;
 int x = x1, y = y1;
 float err = 0.5;
 while (x++ <= x2)
 {
    mvaddch(y, x, '*');
    err += delta;
    if (err > 1)
    {
       err -= 1;
       y++;
    }
 }

 Step 3) Differential implementation (float version /wo division)

 int dx = x2 - x1;
 int dy = y2 - y1;
 int x = x1, y = y1;
 float err = 0.5*dx;
 while (x++ <= x2)
 {
    mvaddch(y, x, '*');
    err += dy;
    if (err > dx)
    {
       err -= dx;
       y++;
    }
 }

 Step 4) Differential implementation (int version)

 int dx = x2 - x1;
 int dy = y2 - y1;
 int x = x1, y = y1;
 int err = dx;
 while (x++ <= x2)
 {
    mvaddch(y, x, '*');
    err += 2*dy;
    if (err > 2*dx)
    {
       err -= 2*dx;
       y++;
    }
 }

 Step 5) Differential implementation (negated version)

 int dx = x2 - x1;
 int dy = y2 - y1;
 int x = x1, y = y1;
 int err = dx;
 while (x++ <= x2)
 {
    mvaddch(y, x, '*');
    err -= dy<<1;
    if (err < 0)
    {
       err += dx<<1;
       y++;
    }
 }

 Step 6) Universal implementation (Bresenham algorithm)

 int dx = x2 - x1;
 int dy = y2 - y1;
 if (abs(dx) > abs(dy))
 {
    // x is fast direction
    ... horizontal case
 }
 else
 {
    // y is fast direction
    ... symmetrical vertical case
 }

*/

// draw a line from position (x1, y1) to (x2, y2)
void draw_line(int y1, int x1, int y2, int x2,
               int ch)
{
   WINDOW *w = W?W:stdscr;

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

   // determine line character
   int c = ch;
   if (c < 0)
   {
      if (dx > 2*dy) c = ACS_HLINE;
      else if (dy > 2*dx) c = ACS_VLINE;
      else if (ix*iy > 0) c = '\\';
      else c = '/';
   }

   // loop along fast direction
   int x = x1, y = y1;
   int err = fast;
   int i = 0;
   while (TRUE)
   {
      // determine background cell
      int b = mvwinch(w, y, x) & A_CHARTEXT;

      // background check
      if (b == ' ')
         mvwaddch(w, y, x, c);

      // end check
      if (i++ == fast)
         break;

      // incremental error
      err -= slow<<1;

      // incremental step
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

// draw a frame from position (y1, x1) to (y2, x2)
void draw_frame(int y1, int x1, int y2, int x2,
                int ch)
{
   WINDOW *w = W?W:stdscr;

   if (x2 > x1 && y2 > y1)
   {
      if (x2 - x1 > 1)
      {
         draw_line(y1, x1+1, y1, x2-1, ch);
         draw_line(y2, x1+1, y2, x2-1, ch);
      }

      if (y2 - y1 > 1)
      {
         draw_line(y1+1, x1, y2-1, x1, ch);
         draw_line(y1+1, x2, y2-1, x2, ch);
      }

      if (peek(y1, x1) == ' ')
         mvwaddch(w, y1, x1, ch>=0?ch:ACS_ULCORNER);

      if (peek(y1, x2) == ' ')
         mvwaddch(w, y1, x2, ch>=0?ch:ACS_URCORNER);

      if (peek(y2, x1) == ' ')
         mvwaddch(w, y2, x1, ch>=0?ch:ACS_LLCORNER);

      if (peek(y2, x2) == ' ')
         mvwaddch(w, y2, x2, ch>=0?ch:ACS_LRCORNER);
   }
}

// Bresenham's circle drawing algorithm
void draw_circle(int yc, int xc, int r,
                 int ch, double aspect)
{
   draw_ellipse(yc, xc, r, aspect*r + 0.5, ch, aspect);
}

// helper for Bresenham's ellipse drawing algorithm
void draw_points(int yc, int xc, int y, int x,
                 int ay, int ax, double aspect,
                 int ch)
{
   int c = ch;
   if (c < 0)
   {
      if (2*ay*x < aspect*ax*y) c = ACS_HLINE;
      else if (ay*x < 2*aspect*ax*y) c = '/';
      else c = ACS_VLINE;
   }

   mvaddch(yc+y, xc+x, c);
   mvaddch(yc+y, xc-x, c=='/'?'\\':c);
   mvaddch(yc-y, xc+x, c=='/'?'\\':c);
   mvaddch(yc-y, xc-x, c);
}

// Bresenham's ellipse drawing algorithm
void draw_ellipse(int yc, int xc, int ay, int ax,
                  int ch, double aspect)
{
   {
      int x = ax, y = 0;
      int dx = ay*ay*(1 - 2*ax), dy = ax*ax;
      int dx2 = 2*ax*ax, dy2 = 2*ay*ay;
      int e = 0, ex = dy2*ax, ey = 0;

      while (ex >= ey)
      {
         draw_points(yc, xc, y, x, ay, ax, aspect, ch);

         y++;
         ey += dx2;
         e += dy;
         dy += dx2;

         if (2*e + dx > 0)
         {
            x--;
            ex -= dy2;
            e += dx;
            dx += dy2;
         }
      }
   }

   {
      int x = 0, y = ay;
      int dx = ay*ay, dy = ax*ax*(1 - 2*ay);
      int dx2 = 2*ax*ax, dy2 = 2*ay*ay;
      int e = 0, ex = 0, ey = dx2*ay;

      while (ex <= ey)
      {
         draw_points(yc, xc, y, x, ay, ax, aspect, ch);

         x++;
         ex += dy2;
         e += dx;
         dx += dy2;

         if (2*e + dy > 0)
         {
            y--;
            ey -= dx2;
            e += dy;
            dy += dx2;
         }
      }
   }
}

// peek at location (y, x) and return the displayed character
int peek(int y, int x)
{
   WINDOW *w = W?W:stdscr;

   return(mvwinch(w, y, x) & A_CHARTEXT);
}

// return wide keycode
int get_keycode()
{
   return(wgetch(stdscr));
}

// decode wide keycode
void cursor_keycode(int keycode,
                    int *dx, unsigned int *dy)
{
   *dx = *dy = 0;

   if (keycode == KEY_LEFT)
   {
      *dx = -1;
      *dy = 0;
   }
   else if (keycode == KEY_RIGHT)
   {
      *dx = 1;
      *dy = 0;
   }
   else if (keycode == KEY_UP)
   {
      *dx = 0;
      *dy = -1;
   }
   else if (keycode == KEY_DOWN)
   {
      *dx = 0;
      *dy = 1;
   }
}

// helper: allocate string buffer
char *get_string_buffer(int n)
{
   n++; // for terminating nul character

   if (buffer_size == 0) buffer_size = 1;
   while (n > buffer_size) buffer_size *= 2;

   if (string_buffer) delete string_buffer;
   string_buffer = new char[buffer_size];

   return(string_buffer);
}
