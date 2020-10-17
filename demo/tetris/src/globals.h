#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include <sound.h>

#define TILE_SIZE 16

extern char *strdup(const char *);

SDL_Joystick *joystick;
int joystick_repeat_delay;

#endif
