#include "NPC.h"

void NPC_init(NPC *p_NPC, tileset *p_tileset_NPC, SDL_Renderer *renderer)
{
    sprite *p_sprite = malloc(sizeof(sprite));
    p_sprite->vel_x = 0;
    p_sprite->vel_y = 0;
    p_sprite->p_tileset = p_tileset_NPC;
    p_sprite->animation_current = DOWN;
    p_NPC->p_sprite = p_sprite;
    p_NPC->p_event = NULL;
}

void NPC_free(NPC *p_NPC)
{
    free(p_NPC->p_sprite);
}