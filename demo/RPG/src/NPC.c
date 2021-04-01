#include "NPC.h"

void NPC_init(NPC *p_NPC, tileset *p_tileset, SDL_Renderer *renderer)
{
    p_NPC->o_sprite.p_tileset = p_tileset;
    p_NPC->o_sprite.animation_current = DOWN;
}
