#ifndef NPC_H
#define NPC_H

#include <sprite.h>

#include "event.h"
#include "utils.h"

typedef struct t_NPC
{
    sprite o_sprite;
    event *p_event;
} NPC;

void NPC_init(NPC *p_NPC, tileset *p_tileset, SDL_Renderer *renderer);

#endif
