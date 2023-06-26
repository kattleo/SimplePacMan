#include "gfx/gfx.h"
#include "gfx/gridfont.h"
#include "gfx/sound.h"
#include "game.h"
#include "gfx/util.h"
#include "gfx/scrollarea.h"
#include "gfx/gridarea.h"

static int sx = 90;
static int sy = 45;

static char c;

class PacMan {
private:
    int pacman_sprite_id;
    double pacman_x;
    double pacman_y;
    double pacman_vel_x;
    double pacman_vel_y;

    void init_pacman_sprite(int id) {
        int *pacsprite[4];
        const char sprite2[] =  "t.--.t"
                                "/ _.-\'"    
                                "\\  \'-."     
                                "t\'--\'t";

        pacsprite[0] = convert_char_text(sprite2, 6, 4, 't', true);
        enable_sprite(id, 6, 4, false, true);
        set_sprite_data(id, 6, 4, pacsprite[0]);
    }

public:
    PacMan(int id, double x, double y) {
        init_pacman_sprite(id);
        pacman_sprite_id = id;

        pacman_x = x;
        pacman_y = y;
        pacman_vel_x = 1;
        pacman_vel_y = 0;

    }

    void move() {
    if(get_cell(pacman_x + pacman_vel_x * 3, pacman_y + pacman_vel_y * 2) != ' ') {
        pacman_vel_x = 0;
        pacman_vel_y = 0;
    }
    
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

    pacman_x += pacman_vel_x;
    pacman_y += pacman_vel_y;

    center_sprite_position(pacman_sprite_id, pacman_x, pacman_y);
}
};

class Ghost {
private:
    int ghost_sprite_id;
    double ghost_x;
    double ghost_y;
    double ghost_vel_x;
    double ghost_vel_y;

    void init_ghost_sprite(int id) {
        int *ghostsprite[4];
        const char sprite1[] =  " .-. "
                            "| OO|"
                            "\'xxx\'";       
        ghostsprite[0] = convert_char_text(sprite1, 5, 3, ' ', true);
        enable_sprite(id, 5, 3, false, true);
        set_sprite_data(id, 5, 3, ghostsprite[0]);
    }
        
public:
    Ghost(int id, double x, double y) {
        init_ghost_sprite(id);
        ghost_sprite_id = id;

        ghost_x = x;
        ghost_y = y;
        ghost_vel_x = 1;
        ghost_vel_y = 0;

    }

    void move() {
        if (get_cell(ghost_x + ghost_vel_x * 3, ghost_y + ghost_vel_y * 3) != ' ') {
            if (ghost_vel_x != 0) {
                ghost_vel_x = 0;
                float random = rnd();
                if (random < 0.5) {
                    ghost_vel_y = -1;
                } else {
                    ghost_vel_y = 1;
                }
            } else if (ghost_vel_y != 0) {
                ghost_vel_y = 0;
                float random = rnd();
                if (random < 0.5) {
                    ghost_vel_x = -1;
                } else {
                    ghost_vel_x = 1;
                }
            }
        }

        ghost_x += ghost_vel_x;
        ghost_y += ghost_vel_y;

        center_sprite_position(ghost_sprite_id, ghost_x, ghost_y);
    }
};

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
    GAME_LOOP = 1,
    GAME_OUTRO = 2
};

// game state: actual game phase
static GAME_STATE state = GAME_INTRO;

static PacMan pacman(1, sx/2, sy/2);
static Ghost ghost1(2, sx/2, sy/2);
static Ghost ghost2(3, sx/2, sy/2);

void game_init() {
    set_area_size(sx, sy);
    set_window_size(COLS, LINES);

    draw_borders();
    //filling background
    //set_grid_size(sx,sy);


    init_font();
    init_grid_font();
    init_color();
}

static int pacman_width = 8;
static int pacman_height = 5;

void draw_borders() {
    //Render Top Left Quarter
    //Top Cube
    render_line(sx/2 - pacman_width/2, 0, sx/2 - pacman_width/2, pacman_height);
    render_line(sx/2 - pacman_width/2, pacman_height, sx/2, pacman_height);

    //Left-Corner
    render_line(pacman_width, pacman_height, pacman_width + sx / 5, pacman_height);
    render_line(pacman_width, pacman_height, pacman_width, sy/2- pacman_height /2);
    render_line(pacman_width, sy/2- pacman_height /2, pacman_width + sx / 10,  sy/2- pacman_height /2);

    //Middle T
    render_line(sx/2, pacman_height * 2, sx/2 - (2 * pacman_width), pacman_height*2);
    render_line(sx/2, pacman_height * 2, sx/2, sy/3);
    enable_sprite(8, sx/2, sy/2, false, true);
    mirror_sprite_horizontal(8, true);
    // render_line(pacman_width, pacman_height, pacman_width, sy/2- pacman_height /2);

    // render_line(pacman_width, pacman_height, sx/2 - pacman_width /2, pacman_height);
    // render_line(pacman_width, pacman_height, pacman_width, sy/2- pacman_height /2);


    
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
        use_color(5);
        draw_grid_text(ty - get_grid_char_lines()/2, tx - strlen(text)*get_grid_char_cols()/2, text);

        // print help and start text
        const char help[] = "press q to quit!";
        const char starting[] = "press s to start!"; 
        use_color(2);
        use_attr_blink();
        use_attr_bold();
        mvprintw(sy-5, COLS - strlen(text)*get_grid_char_cols()/2 - 5, help);
        mvprintw(sy-5, 5, starting);
        use_attr_normal();
        draw_line(sy - 10, 0, sy - 10, COLS, '-');
        draw_line(sy - 11, 0, sy - 11, COLS, '-');

    }
    else if (state == GAME_LOOP)
    {
        // render game area
        center_window(sx / 2, sy / 2);
        render_frame(0, 0, sx-1, sy-1);

        refresh();

        // Moving PacMan
        pacman.move();

        //Moving Ghost
        ghost1.move();
        ghost2.move();
        
        //flood fill
        //flood_fill_grid(0, 0, 'o');
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


     


