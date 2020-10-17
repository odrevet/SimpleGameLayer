#ifndef CALLBACKS_USER_H
#define CALLBACKS_USER_H

#include <SDL.h>

#include <UI/dialog.h>

#include "hotspot.h"

#define CALLBACK_USER_COUNT 2
void (*callbacks_user[CALLBACK_USER_COUNT]) (hotspot *p_hotspot, void *param, SDL_Renderer *renderer);


void assign_callbacks();

#endif
