#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "game.h"
#include "gfx/util.h"
#include "gfx/scrollarea.h"
#include "gfx/gridarea.h"


static char c;

static int sx = 60;
static int sy = 30;

static double pacman_x;
static double pacman_y;
static double pacman_vel_x = 0.1;
static double pacman_vel_y = 0.1;

static double ghost_x;
static double ghost_y;
static double ghost_vel_x = 0.1;
static double ghost_vel_y = 0.1;

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

void move_pacman();

void move_ghost();

void game_init() {
    sx = 60, sy = 30;
    set_area_size(sx, sy);
    set_window_size(COLS, LINES);
    render_frame(0, 0, sx-1, sy-1);
    pacman_x = 0;
    pacman_y = 0;
}

// render a single frame
void render_frame()
{
    // render the actual frame depending on the state
    if (state == GAME_INTRO)
    {
//INTRO start
   // print centered text with 5x3 grid font
        const char text[] = "PACMAN!";
        int tx = COLS/2;
        int ty = LINES/2;
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
        

        //INTRO end
    }
    else if (state == GAME_LOOP)
    {
        // render game area
        center_window(sx / 2, sy / 2);
        refresh();

        set_cell(20, 20, 'P');
        set_cell(20, 21, 'P');
        set_cell(20, 22, 'P');
        set_cell(20, 23, 'P');

        // Moving PacMan
        move_pacman();

        //Moving Ghost
        move_ghost();

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
    pacman_x += pacman_vel_x;
    pacman_y += pacman_vel_y;

    set_cell(pacman_x - pacman_vel_x, pacman_y - pacman_vel_y, ' ');
    set_cell(pacman_x, pacman_y, 'a');

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

void move_ghost() {
    set_cell(ghost_x - ghost_vel_x, ghost_y - ghost_vel_y, ' ');
    set_cell(ghost_x, ghost_y, 'G');
    
    ghost_x += ghost_vel_x;
    ghost_y += ghost_vel_y;
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
            clear();
            }
        
    }
    else if (state == GAME_LOOP)
    {   
        
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