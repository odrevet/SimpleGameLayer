#ifndef NPC_H
#define NPC_H

#include <sprite.h>

#include "utils.h"

// forward declaration
typedef struct t_game game;
typedef struct t_event event;

typedef struct t_NPC
{
    sprite o_sprite;
    event *p_event;
} NPC;

void NPC_init(NPC *p_NPC, tileset *p_tileset_NPC, SDL_Renderer *renderer);

#endif
