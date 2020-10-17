#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <SDL.h>

#include "hotspot.h"

void callback_link(hotspot *p_hotspot, void *param, SDL_Renderer *renderer);
void callback_text(hotspot *p_hotspot, void *param, SDL_Renderer *renderer);

#endif
