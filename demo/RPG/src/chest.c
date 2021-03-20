#include "chest.h"

void chest_init(chest *p_chest, tileset *p_tileset, SDL_Renderer *renderer)
{
    p_chest->o_sprite.p_tileset = p_tileset;
    p_chest->p_event = NULL;
}