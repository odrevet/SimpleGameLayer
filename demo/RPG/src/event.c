#include "event.h"

void event_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  printf("event func\n");
}

void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game, (char*)p_event->p_param, renderer);
}

void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  event_param_warp *p_event_param_warp = (event_param_warp*) p_event->p_param;

  p_game->p_hero->p_sprite->x = p_event_param_warp->index_x * p_game->p_level->p_map->tile_width;
  p_game->p_hero->p_sprite->y = p_event_param_warp->index_y * p_game->p_level->p_map->tile_height;

  if (p_event_param_warp->p_level_addr != p_game->p_level_addr)
  {
    // assign the current loaded level to the destination level
    p_game->p_level_addr = p_event_param_warp->p_level_addr;

    // clear level
    p_game->p_level->event_count = 0;
    free(p_game->p_level->p_event);
    p_game->p_level->p_event = NULL;
    p_game->p_level->NPC_count = 0;
    free(p_game->p_level->p_NPC);
    p_game->p_level->p_NPC = NULL;

    // callback function
    p_game->p_level_addr(p_game, renderer);
  }
}