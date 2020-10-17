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

typedef void (*on_action)(NPC *, game *, SDL_Renderer *);

typedef struct t_NPC
{
    sprite *p_sprite;
    on_action p_on_action_callback;
    void *p_on_action_param;
} NPC;

void NPC_init(NPC *p_NPC, image *p_image_NPC, SDL_Renderer *renderer);

void NPC_free(NPC *p_NPC);

#endif
