#ifndef SOUND_H
#define SOUND_H

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_mixer.h>

char sound_init();

Mix_Music *music_load(char *full_path);

void music_play(Mix_Music *music);

/**
    @brief Load a sample into memory
    @note wav only
*/
Mix_Chunk *sample_load(char *full_path);

void sample_play(Mix_Chunk *);

#endif
