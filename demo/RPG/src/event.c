#include "event.h"

void event_init(event *p_event, event_trigger event_trigger, event_type event_type, void *p_param, int index_x, int index_y)
{
  p_event->p_param = p_param;
  p_event->has_triggered = false;
  p_event->o_event_trigger = event_trigger;
  p_event->o_event_type = event_type;
  p_event->index_src_x = index_x;
  p_event->index_src_y = index_y;
}

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
  case EVENT_TYPE_MONEY:
    event_money_exec(p_event, p_game, renderer);
    break;

  default:
    break;
  }
}

void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game, (char *)p_event->p_param, renderer);
}

void event_money_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  int *amount = (int*)p_event->p_param;
  p_game->o_hero.money += *amount;

  char message[32];
  snprintf(message, 32, "YOU GOT %d GOLD COINS!", *amount);
  dialog_box_show(p_game, (char *)message, renderer);
}

void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  event_param_warp *p_event_param_warp = (event_param_warp *)p_event->p_param;

  p_game->o_hero.o_sprite.x = p_event_param_warp->index_x * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_game->o_hero.o_sprite.y = p_event_param_warp->index_y * p_game->o_level.o_tilemap.p_tileset->tile_height;

  if (p_event_param_warp->p_level_addr != p_game->p_level_function)
  {
    p_game->p_level_function = p_event_param_warp->p_level_addr;
    for (int event_index = 0; event_index < p_game->o_level.event_count; event_index++)
    {
      event_free(p_game->o_level.p_event + event_index);
    }
    level_free(&p_game->o_level);
    level_init(&p_game->o_level);
    p_game->p_level_function(p_game, renderer);
  }
}

void event_free(event *p_event)
{
  switch (p_event->o_event_type)
  {
  case EVENT_TYPE_WARP:
    free((event_param_warp *)p_event->p_param);
    break;
  default:
    break;
  }
}
