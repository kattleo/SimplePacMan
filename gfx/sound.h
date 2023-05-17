// SDL sound for NCurses
// (c) 2020 by Stefan Roettger

#pragma once

//! init SDL
bool sound_init();

//! sound detected
bool sound_detected();

//! enable or disable all sound channels
void configure_sound(bool yes);

//! play WAV file with SDL
//! * sound volume is specified int the range [0,1]
//! * returns the playing sound channel
int sound_play(const char *filename, float volume = 1);

//! check if a sound channel is still playing
bool sound_playing(int channel);

//! stop playing a sound channel
void sound_stop(int channel);

//! mute all sound channels
void sound_mute(bool yes = true);

//! exit SDL
void sound_exit();
