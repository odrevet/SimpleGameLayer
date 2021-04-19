#include "levels.h"

/*
void level_town(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/town.map", renderer);

  // Events
  p_game->o_level.event_count = 2;
  event *p_event = calloc(p_game->o_level.event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 1;
  p_event_param_warp->index_y = 8;
  p_event_param_warp->p_level_filepath = "data/overworld.level";
  event_init(p_event + 0, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp, 19, 8);

  event_param_warp *p_event_param_warp_house = malloc(sizeof(event_param_warp));
  p_event_param_warp_house->index_x = 4;
  p_event_param_warp_house->index_y = 7;
  p_event_param_warp_house->p_level_filepath = "data/house.level";
  event_init(p_event + 1, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp_house, 14, 5);

  p_game->o_level.p_event = p_event;
}
*/