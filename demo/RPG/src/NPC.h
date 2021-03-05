#ifndef NPC_H
#define NPC_H

#include <sprite.h>

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

// forward declaration
typedef struct t_NPC NPC;
typedef struct t_game game;
typedef struct t_event event;

typedef void (*on_action)(NPC *, game *, SDL_Renderer *);

typedef struct t_NPC
{
    sprite o_sprite;
    event *p_event;
} NPC;

void NPC_init(NPC *p_NPC, tileset *p_tileset_NPC, SDL_Renderer *renderer);

#endif
