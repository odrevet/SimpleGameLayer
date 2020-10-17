#include "warp.h"

void warp_exec(warp *p_warp, game *p_game, SDL_Renderer *renderer)
{
  p_game->p_hero->p_sprite->x = p_warp->index_x * p_game->p_level->p_map->tile_width;
  p_game->p_hero->p_sprite->y = p_warp->index_y * p_game->p_level->p_map->tile_height;

  if (p_warp->p_level_dest != p_game->p_level_addr)
  {
    // assign the current loaded level to the destination level
    p_game->p_level_addr = p_warp->p_level_dest;

    // clear level memory
    p_game->p_level->warp_cout = 0;
    free(p_game->p_level->p_warp);
    p_game->p_level->p_warp = NULL;
    p_game->p_level->NPC_cout = 0;
    free(p_game->p_level->p_NPC);
    p_game->p_level->p_NPC = NULL;

    // exec the callback function
    p_game->p_level_addr(p_game, renderer);
  }
}
