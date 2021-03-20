#ifndef CHEST_H
#define CHEST_H

#include <stdlib.h>

#include <SDL.h>

#include <sprite.h>

#include "event.h"

typedef struct t_chest
{
    int id;
    bool is_open;
    sprite o_sprite;
    event *p_event;
} chest;

void chest_init(chest *p_chest, tileset *p_tileset, SDL_Renderer *renderer);

#endif