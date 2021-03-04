#include "event.h"

void event_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  p_event->has_triggered = true;
  switch (p_event->o_event_type)
  {
  case EVENT_TYPE_FUNCTION:
  {
    event_callback p_event_callback = (event_callback)p_event->p_param;
    p_event_callback(p_game, renderer);
  }
  break;
  case EVENT_TYPE_TEXT:
    event_text_exec(p_event, p_game, renderer);
    break;
  case EVENT_TYPE_WARP:
    event_warp_exec(p_event, p_game, renderer);
    break;

  default:
    break;
  }
}

void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game, (char *)p_event->p_param, renderer);
}

void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  event_param_warp *p_event_param_warp = (event_param_warp *)p_event->p_param;

  p_game->o_hero.p_sprite->x = p_event_param_warp->index_x * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_game->o_hero.p_sprite->y = p_event_param_warp->index_y * p_game->o_level.o_tilemap.p_tileset->tile_height;

  if (p_event_param_warp->p_level_addr != p_game->p_level_function)
  {
    p_game->p_level_function = p_event_param_warp->p_level_addr;
    level_free(&p_game->o_level);
    level_init(&p_game->o_level);
    p_game->p_level_function(p_game, renderer);
  }
}