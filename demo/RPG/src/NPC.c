#include "NPC.h"

void NPC_init(NPC *p_NPC, tileset *p_tileset_NPC, SDL_Renderer *renderer)
{
    p_NPC->o_sprite.p_tileset = p_tileset_NPC;
    p_NPC->o_sprite.animation_current = DOWN;
    p_NPC->p_event = NULL;
}
