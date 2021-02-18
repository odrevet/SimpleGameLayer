#include "event.h"

void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game, (char *)p_event->p_param, renderer);
}

void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  event_param_warp *p_event_param_warp = (event_param_warp *)p_event->p_param;

  p_game->p_hero->p_sprite->x = p_event_param_warp->index_x * p_game->p_level->p_map->tile_width;
  p_game->p_hero->p_sprite->y = p_event_param_warp->index_y * p_game->p_level->p_map->tile_height;

  if (p_event_param_warp->p_level_addr != p_game->p_level_function)
  {
    p_game->p_level_function = p_event_param_warp->p_level_addr;
    level_free(p_game->p_level);
    level_init(p_game->p_level);
    p_game->p_level_function(p_game, renderer);
  }
}