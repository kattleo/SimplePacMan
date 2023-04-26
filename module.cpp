#include <stdio.h> 
#include <unistd.h> 
#include <ncurses.h> 
#include <stdlib.h>
#include "module.h"

void print_bird(int y, int x){

    mvaddstr(y, x+4, "\\");
    mvaddstr(y+1, x+4, "|\\");
    mvaddstr(y+2, x, ">=====0-");
    mvaddstr(y+3, x+4, "|/");
    mvaddstr(y+4, x+4, "/");

}

void mssleep(float ms){

   int us = ms*1000; // micro secs
   usleep(us);

   }
   
static int y = 10;
static int x = 10;
static int dy = 1;
static int dx = 1;

void render_frame(){

   clear();

   print_bird(y, x);

   print_bird(y + 5, x + 2);

   print_bird(y - 5, x + 4);

   x = x + dx;
   if (x >= COLS - 1 || x <= 0){
      dx = -dx;
   }
   y = y + dy;
   if (y >= LINES - 1 || y >= 0){
      dy = -dy;
   }

   }

static float cur_time = 0;

void game_loop(){

   const float fps = 10;
   float dt = 1.0/fps;
   float ms = dt * 1000;

   while(TRUE){
      cur_time += dt;
      render_frame();
      refresh();
      mssleep(ms);

      int c = getch();
         if(c == 'q'){
            return;
         }
      }
   }

double get_cur_time(){

    return(cur_time);

}




