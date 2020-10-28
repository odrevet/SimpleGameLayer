#ifndef CALLBACKS_USER_H
#define CALLBACKS_USER_H

#include <SDL.h>

#include <UI/dialog.h>

#include "hotspot.h"

#define CALLBACK_USER_COUNT 2

void callback_user_click_bedroom_door(hotspot *p_hotspot, void *param, SDL_Renderer *renderer);
void callback_user_click_desk(hotspot *p_hotspot, void *param, SDL_Renderer *renderer);

#endif
