#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "game.h"
#include "gfx/util.h"
#include "gfx/scrollarea.h"
#include "gfx/gridarea.h"


static char c;

static double pacman_x;
static double pacman_y;
static double pacman_vel_x = 0.2;
static double pacman_vel_y = 0.2;

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
        const char starting[] = "press s to start!";
        use_attr_bold(); // enable bold attribute
        use_attr_blink(); // enable blink attribute
        use_color(2); // index 2 equals red
        mvprintw(max_y-2, 1, help);
        mvprintw(max_y-2, max_x - 20, starting);
        use_color(); // default equals white
        use_attr_normal(); // disable all attributes

        // init PacMan starting point
        pacman_x = COLS/2;
        pacman_y = LINES/2;

        //INTRO end
    }
    else if (state == GAME_LOOP)
    {
        clear();

        // game area setup
        int sx = 60, sy = 30;
        set_area_size(sx, sy);
        set_window_size(COLS, LINES);

        // render game area
        render_frame(0, 0, sx-1, sy-1);
        render_text_format(2, sy-6, "window area:\ncols=%d lines=%d", COLS, LINES);
        render_text_format(2, sy-3, "scrollable area:\ncols=%d lines=%d", sx, sy);
        set_cell_offset(6, 3);
        set_cell_offset();
        refresh();

        // Moving PacMan
        move_pacman();
        

        // // game loop
        // do
        // {
        //     // scroll game area
        //     center_window(sx/2, sy/2);

        //     // refresh screen
        //     refresh();

        //     // wait until next frame
        //     msleep(100); // 100 ms = 10 fps
        // }
    }
    else if (state == GAME_OUTRO){
        clear();
        char text[] = "BYE BYE!!!!"; // intro text to be rendered
        mvprintw((LINES-1)/2, (COLS-1)/2-strlen(text)/2, text); // render centered text

    }

    refresh();
}

void move_pacman() {
    mvaddstr(pacman_y, pacman_x, "O");

    switch(c) {
        case 'w':
            pacman_vel_y = -1;
            pacman_vel_x = 0;
            break;
        case 'a':
            pacman_vel_y = 0;
            pacman_vel_x = -1;
            break;
        case 's':
            pacman_vel_y = 1;
            pacman_vel_x = 0;
            break;
        case 'd':
            pacman_vel_y = 0;
            pacman_vel_x = 1;
            break;
    }
    
    
}

// update the game state
// * 'q' quits the game

bool update_state()
{   
    static float outtime = 0;
    // state check cascade
    if (state == GAME_INTRO)
    {
        
        if (c == 's'){
            state = GAME_LOOP;
            }
        
    }
    else if (state == GAME_LOOP)
    {   
        pacman_x += pacman_vel_x;
        pacman_y += pacman_vel_y;
        if (c == 'q'){
            state = GAME_OUTRO;
            outtime = get_elapsed() + 3;
            }
    }
    else if (state == GAME_OUTRO){
        if(get_elapsed() > outtime){
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
        refresh();

        // update frame counter and elapsed time
        frame++;
        elapsed += dt;

        c = getch();
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