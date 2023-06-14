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
static double pacman_vel_x;
static double pacman_vel_y;

static double ghost_x;
static double ghost_y;
static double ghost_vel_x = 1;
static double ghost_vel_y = 0;

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
    sx = 80, sy = 40;
    set_area_size(sx, sy);
    set_window_size(COLS, LINES);

    init_font();
    init_grid_font();

    // create sprites
       // create sprites
   int *sprite[4];
   const char sprite1[] =   "/^^^\\"
                            "|o o|"
                            "\\/\\/\\";
   sprite[0] = convert_char_text(sprite1, 5, 3, ' ', true);
   enable_sprite(1, 5, 3, false, true);
   set_sprite_data(1, 5, 3, sprite[0]);
   
//    int *sprite[3];
//    const char sprite1[] =   "/^^^\\"
//                             "|o o|"
//                             "\\/\\/\\";
//    sprite[0] = convert_char_text(sprite1, 10, 4, ' ', true);
//    sprite[1] = convert_char_text(sprite1, 10, 4, ' ', true);
//    sprite[2] = convert_char_text(sprite1, 10, 4, ' ', true);
//    enable_sprite(1, 10, 4, false, true);
//    set_sprite_data(1, 10, 4, sprite[2]);


    //init sprite
    //draw_grid_text(3, 3, "g");
    //mvaddch(2, 2, 'a');
    //mvaddch(3, 2, 'b');

    //fill_cell_area(2, 2, 2, 2, 'a');
    //int *data = get_cell_area(1, 1, 4, 4, ' ');
    //enable_sprite(1, 4, 4);
    //set_sprite_data(1, 4, 4, data);
    //clear();
    //refresh();

    // init PacMan starting point
    pacman_x = sx/2;
    pacman_y = sy/2;

    ghost_x = 2;
    ghost_y = 2;
}

// render a single frame
void render_frame()
{
    // render the actual frame depending on the state
    if (state == GAME_INTRO)
    {
//INTRO start
   // print centered text with 5x3 grid font
        const char text[] = "PACMAN";
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

        //INTRO end
    }
    else if (state == GAME_LOOP)
    {
        // render game area
        center_window(sx / 2, sy / 2);
        render_frame(0, 0, sx-1, sy-1);
        refresh();

        // Moving PacMan
        move_pacman();

        //Moving Ghost
        move_ghost();
        
        //center_sprite_position(1, 10, 10);
    }
    else if (state == GAME_OUTRO){
        clear();
        const char text[] = "BYE BYE!";
        int tx = COLS/2;
        int ty = LINES/2;
        draw_grid_text(ty - get_grid_char_lines()/2, tx - strlen(text)*get_grid_char_cols()/2, text);

    }
    refresh();
}

void move_pacman() {
    if(get_cell(pacman_x + pacman_vel_x, pacman_y + pacman_vel_y) != ' ') {
        pacman_vel_x = -pacman_vel_x;
        pacman_vel_y = -pacman_vel_y;
    }
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
    if(get_cell(ghost_x + ghost_vel_x * 3, ghost_y + ghost_vel_y * 2) != ' ') {
        if(ghost_vel_x != 0) {
            ghost_vel_x = 0;
        float random = rnd();
        if(random < 0.5) {
            ghost_vel_y = -1;
        } else {
            ghost_vel_y = 1;
        }  

        } else if(ghost_vel_y != 0) {
            ghost_vel_y = 0;
        float random = rnd();
        if(random < 0.5) {
            ghost_vel_x = -1;
        } else {
            ghost_vel_x = 1;
        }  
        }
    }
    

    ghost_x += ghost_vel_x;
    ghost_y += ghost_vel_y;

    center_sprite_position(1, ghost_x, ghost_y);
    //set_cell(ghost_x - ghost_vel_x, ghost_y - ghost_vel_y, ' ');
    //set_cell(ghost_x, ghost_y, 'g');

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