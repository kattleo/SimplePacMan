#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "game.h"
#include "gfx/util.h"

// game constant: frames per second
static float fps = 10;

// game state: actual frame number
static int frame = 0;

// game state: elapsed time
static double elapsed = 0;

static int max_x = 0, max_y = 0;
// definition of game phases
enum GAME_STATE
{
    GAME_INTRO = 0,
    GAME_LOOP = 1,
    GAME_OUTRO = 2
};

// game state: actual game phase
static GAME_STATE state = GAME_INTRO;

// prototype of getter function
double get_elapsed();

int get_state();

// render a single frame
void render_frame()
{
    // clear the actual frame
    //clear();

    // render the actual frame depending on the state
    getmaxyx(stdscr, max_y, max_x);
    mvhline(max_y-3, 0, ACS_HLINE, max_x);
    if (state == GAME_INTRO)
    {
//INTRO start
   // print centered text with 5x3 grid font
   const char text[] = "PACMAN!";
   int tx = max_x/2;
   int ty = (max_y-4)/2+1;
   init_grid_font();
   draw_grid_text(ty - get_grid_char_lines()/2, tx - strlen(text)*get_grid_char_cols()/2, text);

   // print help text
   const char help[] = "press q to quit!";
   use_attr_bold(); // enable bold attribute
   use_attr_blink(); // enable blink attribute
   use_color(2); // index 2 equals red
   mvprintw(max_y-2, 1, help);
   use_color(); // default equals white
   use_attr_normal(); // disable all attributes
   //INTRO end
    }
    else if (state == GAME_LOOP)
    {
#if 0
        ... // your own game code
#else
        char text[] = "YOU ARE NOW IN THE GAME!!!";
        mvprintw((LINES-1)/2, (COLS-1)/2-strlen(text)/2, text);
#endif
    }
    else if (state == GAME_OUTRO){
        char text[] = "DUMMES OUTRO!!!!"; // intro text to be rendered
        mvprintw((LINES-1)/2, (COLS-1)/2-strlen(text)/2, text); // render centered text
    }
    refresh();
}

// update the game state
// * 'q' quits the game
bool update_state()
{
    // state check cascade
    if (state == GAME_INTRO)
    {
        if (get_elapsed() > 3){
            state = GAME_LOOP;
            }
    }
    else if (state == GAME_LOOP)
    {
        if (getch() == 'q'){
            state = GAME_OUTRO;
            msleep(5000);
            return true;
            }
    }

    return(false);
}

// the game loop
// * it renders frame after frame
// * updates the state after each frame
// * and sleeps for the remainder of the frame
void game_loop()
{
    float dt = 1/fps; // frame duration
    float ms = dt*1000; // milli seconds

    // the main game loop
    while (true)
    {
        // render a single frame
        render_frame();

        // update frame counter and elapsed time
        frame++;
        elapsed += dt;

        // update the game state
        bool finish = update_state();

        // sleep for the remainder of the frame
        msleep(ms);

        // check for game finish
        if (finish) break;
    }
}

// get elapsed time
double get_elapsed()
{
    return(elapsed);
}

int get_state(){
    if(state == GAME_INTRO){
        return 0;
    }
    else {
        return 1;
    }
}