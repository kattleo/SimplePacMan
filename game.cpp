#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

// game constant: frames per second
static float fps = 10;

// game state: actual frame number
static int frame = 0;

// game state: elapsed time
static double elapsed = 0;

// definition of game phases
enum GAME_STATE
{
    GAME_INTRO = 0,
    GAME_LOOP = 1
};

// game state: actual game phase
static GAME_STATE state = GAME_INTRO;

// prototype of getter function
double get_elapsed();

// render a single frame
void render_frame()
{
    // clear the actual frame
    clear();

    // render the actual frame depending on the state
    if (state == GAME_INTRO)
    {
#if 0
        // your own intro code
#else
        char text[] = "PACMAN IS A GOOD GAME!!!"; // intro text to be rendered
        mvprintw((LINES-1)/2, (COLS-1)/2-strlen(text)/2, text); // render centered text
#endif
    }
    else if (state == GAME_LOOP)
    {
#if 0
        ... // your own game code
#else
        flash(); // just a flash as a starting point
#endif
    }
}

// update the game state
// * 'q' quits the game
bool update_state()
{
    // state check cascade
    if (state == GAME_INTRO)
    {
        if (get_elapsed() > 3)
            state = GAME_LOOP;
    }
    else if (state == GAME_LOOP)
    {
        if (getch() == 'q')
            return(true);
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
