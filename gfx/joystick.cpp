// SDL joystick input for NCurses
// (c) 2020 by Stefan Roettger

#define JOYSTICK_AXIS1 0
#define JOYSTICK_AXIS2 1
#define JOYSTICK_FIRE1 3
#define JOYSTICK_FIRE2 4

#include "joystick.h"

#ifdef HAVE_SDL
#   include <SDL.h>
#endif

#ifdef HAVE_SDL
static bool enabled = true;
static int axis1 = JOYSTICK_AXIS1;
static int invert1 = 1;
static int axis2 = JOYSTICK_AXIS2;
static int invert2 = 1;
static int fire1 = JOYSTICK_FIRE1;
static int fire2 = JOYSTICK_FIRE2;
static const int threshold  = 32768/2;
static SDL_Joystick *joystick = NULL;
#endif

// init joystick
bool joystick_init()
{
#ifdef HAVE_SDL

   if (!SDL_WasInit(SDL_INIT_JOYSTICK))
      SDL_InitSubSystem(SDL_INIT_JOYSTICK);

   joystick = NULL;

   if (enabled)
      if (SDL_NumJoysticks() > 0)
         joystick = SDL_JoystickOpen(0);

   return(joystick);

#endif

   return(false);
}

// joystick detected
bool joystick_detected()
{
#ifdef HAVE_SDL
   return(joystick);
#else
   return(false);
#endif
}

// configure joystick
void configure_joystick(bool on)
{
#ifdef HAVE_SDL
   enabled = on;
#endif
}

// configure joystick axis #1
void configure_axis1(int axis)
{
#ifdef HAVE_SDL
   axis1 = axis;
#endif
}

// configure inverted joystick axis #1
void configure_inverted1(bool invert)
{
#ifdef HAVE_SDL
   invert1 = invert?-1:1;
#endif
}

// configure joystick axis #2
void configure_axis2(int axis)
{
#ifdef HAVE_SDL
   axis2 = axis;
#endif
}

// configure inverted joystick axis #2
void configure_inverted2(bool invert)
{
#ifdef HAVE_SDL
   invert2 = invert?-1:1;
#endif
}

// configure fire button #1
void configure_fire1(int button)
{
#ifdef HAVE_SDL
   fire1 = button;
#endif
}

// configure fire button #2
void configure_fire2(int button)
{
#ifdef HAVE_SDL
   fire2 = button;
#endif
}

// joystick moved left
bool joystick_left()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetAxis(joystick, axis1)*invert1 < -threshold)
         return(true);

#endif

   return(false);
}

// joystick moved right
bool joystick_right()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetAxis(joystick, axis1)*invert1 > threshold)
         return(true);

#endif

   return(false);
}

// joystick moved up
bool joystick_up()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetAxis(joystick, axis2)*invert2 < -threshold)
         return(true);

#endif

   return(false);
}

// joystick moved down
bool joystick_down()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetAxis(joystick, axis2)*invert2 > threshold)
         return(true);

#endif

   return(false);
}

// joystick fire button 1 pressed
bool joystick_fire1()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetButton(joystick, fire1))
         return(true);

#endif

   return(false);
}

// joystick fire button 2 pressed
bool joystick_fire2()
{
#ifdef HAVE_SDL

   SDL_JoystickUpdate();
   if (joystick)
      if (SDL_JoystickGetButton(joystick, fire2))
         return(true);

#endif

   return(false);
}

// decode joystick state
void joystick_state(int *dx, int *dy, int *buttons)
{
   *dx = *dy = *buttons = 0;

   if (joystick_left()) *dx = -1;
   if (joystick_right()) *dx = 1;
   if (joystick_up()) *dy = -1;
   if (joystick_down()) *dy = 1;
   if (joystick_fire1()) *buttons |= 1;
   if (joystick_fire2()) *buttons |= 2;
}

// exit SDL
void joystick_exit()
{
#ifdef HAVE_SDL

   if (SDL_WasInit(SDL_INIT_JOYSTICK))
      SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

   joystick = NULL;

#endif
}
