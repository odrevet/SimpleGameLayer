#ifndef WARP_H
#define WARP_H

#include "game.h"
#include "level.h"

typedef struct t_warp
{  
    level_addr p_level_dest;
    int index_src_x;
    int index_src_y;
    int index_x;
    int index_y;
} warp;

void warp_exec(warp *p_warp, game *p_game, SDL_Renderer *renderer);

#endif