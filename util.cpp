#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "gfx/gridarea.h"
#include "gfx/gridmenu.h"
#include "gfx/math2d.h"
#include "gfx/mouse.h"
#include "gfx/polygon.h"
#include "gfx/scrollarea.h"
#include "gfx/util.h"

void msleep(float ms)
{
    int us = ms*1000; // micro secs
    usleep(us);
}


void init_font(){
      set_grid_char_text('g',
                      "3/^^^\\"
                      "|o o|"
                      "\\/\\/\\", true);


      set_grid_char_text('o',
                           "     "
                           "     "
                           "  o  ");
      set_grid_char_text('1',
                           "     "
                           "     "
                           " o   ");
      set_grid_char_text('2',
                           "     "
                           "o    "
                           "     ");
      set_grid_char_text('3',
                           "     "
                           "o    "
                           "     ");
      set_grid_char_text('4',
                           " o   "
                           "     "
                           "     ");
      set_grid_char_text('5',
                           "  o  "
                           "     "
                           "     ");
      set_grid_char_text('6',
                           "   o "
                           "     "
                           "     ");
      set_grid_char_text('7',
                           "     "
                           "    o"
                           "     ");
      set_grid_char_text('8',
                           "     "
                           "    o"
                           "     ");
      set_grid_char_text('9',
                           "     "
                           "     "
                           "   o ");
      set_grid_animation_string('o', "o123456789");

      set_grid_char_text('#',
                      "#####"
                      "#####"
                      "#####", true);

   }