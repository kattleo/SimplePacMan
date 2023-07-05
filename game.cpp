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
    int sprite_id;
    double x;
    double y;
    int lives;
    int width;
    int height;
    double vel_x;
    double vel_y;

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
    PacMan(int id, double pacman_x, double pacman_y) {
        init_pacman_sprite(id);
        sprite_id = id;

        x = pacman_x;
        y = pacman_y;
        vel_x = 0;
        vel_y = 0;
        lives = 3;
        width = 8;
        height = 5;

    }

    void move() {
    if(get_cell(x + vel_x * 3, y + vel_y * 2) != ' ') {
        vel_x = 0;
        vel_y = 0;
    }
    
    switch(c) {
        case 'w':
            vel_y = -1;
            vel_x = 0;
            break;
        case 'a':
            vel_y = 0;
            vel_x = -1;
            break;
        case 's':
            vel_y = 1;
            vel_x = 0;
            break;
        case 'd':
            vel_y = 0;
            vel_x = 1;
            break;
    }

    x += vel_x;
    y += vel_y;

    center_sprite_position(sprite_id, x, y);
}

    // getter functions
    int get_width() {
        return width;
    }

    int get_height() {
        return height;
    }

    int get_x() {
        return x;
    }

    int get_y() {
        return y;
    }

    int get_lives() {
        return lives;
    }

    // setter functions
    void decrease_lives() {
        lives -= 1;
    }

};

PacMan pacman(1, sx/2, sy/2);

class Ghost {
private:
    int sprite_id;
    double x;
    double y;
    double vel_x;
    double vel_y;

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
    Ghost(int id, double ghost_x, double ghost_y, int ghost_vel_x, int ghost_vel_y) {
        init_ghost_sprite(id);
        sprite_id = id;
        x = ghost_x;
        y = ghost_y;
        vel_x = ghost_vel_x;
        vel_y = ghost_vel_y;
    }

    void move() {
        // handling Border movement
        // handling x axis
        if(vel_x != 0) {
            if(get_cell(x + vel_x * 3, y) != ' ') {
                // case: Both ghost top and bottom are free
                if(get_cell(x, y + vel_y * 3) == ' ' &&
                get_cell(x, y - vel_y * 3) == ' ') {
                    float random = rnd();
                    if (random < 0.5) {
                    vel_y = -1;
                    } else {
                    vel_y = 1;
                    }
                }
                // case: Ghost top is free
                else if(get_cell(x, y - vel_y * 3) == ' ') {
                    vel_y = 1;
                }
                // case: Ghost bottom is free
                else if(get_cell(x, y + vel_y * 3) == ' ') {
                    vel_y = -1;
                }
                vel_x = 0;
            }
        }

        // handling y axis
        if(vel_y != 0) {
            if(get_cell(x, y + vel_y * 3) != ' ') {
                // case: Both ghost left and right are free
                if(get_cell(x + vel_x * 3, y) == ' ' &&
                get_cell(x - vel_x * 3, y) == ' ') {
                    float random = rnd();
                    if (random < 0.5) {
                    vel_x = -1;
                    } else {
                    vel_x = 1;
                    }
                }
                // case: Ghost left is free
                else if(get_cell(x - vel_x * 3, y) == ' ') {
                    vel_x = -1;
                }
                // case: Ghost bottom is free
                else if(get_cell(x + vel_x * 3, y) == ' ') {
                    vel_x = 1;
                }
                vel_y = 0;
            }
            
        }

        // handling colission with PacMan
        if (pacman.get_x() == x && pacman.get_y() == y) {
            pacman.decrease_lives();
        }

        x += vel_x;
        y += vel_y;

        center_sprite_position(sprite_id, x, y);
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

static Ghost ghost1(2, sx/2, sy/2, 1, 0);
static Ghost ghost2(3, sx/2, sy/2, 0, -1);
static Ghost ghost3(4, sx/2, sy/2,-1, 0);
static Ghost ghost4(5, sx/2, sy/2, 0, 1);

void game_init() {
    set_area_size(sx, sy);
    set_window_size(COLS, LINES);

    draw_borders();

    init_font();
    init_grid_font();
    init_color();
}


void draw_borders() {
    static int pacman_width = pacman.get_width();
    static int pacman_height = pacman.get_height();

    //Render Top Left Quarter
    //Top Cube
    render_line(sx/2 - pacman_width/2, 0, sx/2 - pacman_width/2, pacman_height);
    render_line(sx/2 - pacman_width/2, pacman_height, sx/2 + pacman_width/2, pacman_height);
    render_line(sx/2 + pacman_width/2, pacman_height,sx/2 + pacman_width/2, 0);

    //Top Left Corner
    render_line(pacman_width, pacman_height, pacman_width + sx / 5, pacman_height);
    render_line(pacman_width, pacman_height, pacman_width, sy/2- pacman_height /2);
    render_line(pacman_width, sy/2- pacman_height /2, pacman_width + sx / 10,  sy/2- pacman_height /2);

    //Middle T
    render_line( sx/2 - (2 * pacman_width), pacman_height*2, sx/2 + (2 * pacman_width), pacman_height * 2);
    render_line(sx/2, pacman_height * 2, sx/2, sy/3);

    //Render Top Right Quarter
    //Top Right Corner
    render_line(sx - pacman_width, pacman_height, sx - pacman_width - sx/5, pacman_height);
    render_line(sx - pacman_width, pacman_height, sx - pacman_width, sy/2- pacman_height /2);
    render_line(sx - pacman_width, sy/2- pacman_height /2, sx - pacman_width - sx / 10,  sy/2- pacman_height /2);

    //Bottom Left Corner
    render_line(pacman_width, sy/2 + pacman_height, pacman_width + sx / 10, sy/2 + pacman_height);
    render_line(pacman_width, sy/2 + pacman_height, pacman_width, sy - pacman_height);
    render_line(pacman_width, sy - pacman_height, pacman_width + sx / 5, sy - pacman_height);

    //Bottom Right Corner
    render_line(sx-pacman_width, sy/2 + pacman_height, sx - pacman_width - sx / 10, sy/2 + pacman_height);
    render_line(sx - pacman_width, sy/2 + pacman_height, sx - pacman_width, sy - pacman_height);
    render_line(sx - pacman_width, sy - pacman_height,sx - pacman_width - sx / 5, sy - pacman_height);

    //Bottom Cube
    render_line(sx/2 - pacman_width/2, sy, sx/2 - pacman_width/2, sy - pacman_height);
    render_line(sx/2 - pacman_width/2, sy - pacman_height, sx/2 + pacman_width/2, sy - pacman_height);
    render_line(sx/2 + pacman_width/2, sy - pacman_height,sx/2 + pacman_width/2, sy);

    //Bottom T
    render_line(sx/2 - (2 * pacman_width), sy - pacman_height*2, sx/2 + (2 * pacman_width), sy - pacman_height * 2);
    render_line(sx/2, sy - pacman_height * 2 - 1, sx/2, sy - sy/3);
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
        const char start[] = "press s to start!"; 
        use_color(2);
        use_attr_blink();
        use_attr_bold();
        mvprintw(sy-5, COLS - strlen(text)*get_grid_char_cols()/2 - 5, help);
        mvprintw(sy-5, 5, start);
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

        //Moving Ghosts
        ghost1.move();
        ghost2.move();
        ghost3.move();
        ghost4.move();
        
        // Displaying PacMan lives
        const char original_string[] = "Pacman lives: ";

        // Int to Str
        char value_string[2];
        snprintf(value_string, sizeof(value_string), "%d", pacman.get_lives());

        // Size for new String
        const int newSize = strlen(original_string) + strlen(value_string) + 1;

        // Creating new String
        char* result_string = (char*)malloc(newSize * sizeof(char));
        strcpy(result_string, original_string);
        strcat(result_string, value_string);

        mvprintw(sy/2, 0, result_string);
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
        
        if (c == 'q' || pacman.get_lives() == 0){
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


     


