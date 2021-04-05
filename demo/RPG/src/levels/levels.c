#include "levels.h"

void level_overworld(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/overworld.map", renderer);
  level_init_from_file(&p_game->o_level, "data/overworld.level", &p_game->path_music, renderer);
}

void level_cave(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/cave.map", renderer);
  level_init_from_file(&p_game->o_level, "data/cave.level", &p_game->path_music, renderer);

  // Events
  p_game->o_level.event_count = 1;
  event *p_event = calloc(p_game->o_level.event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 2;
  p_event_param_warp->index_y = 2;
  p_event_param_warp->p_level_addr = level_overworld;

  event_init(p_event + 0, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp, 3, 5);

  p_game->o_level.p_event = p_event;
}

void level_town(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/town.map", renderer);

  // Events
  p_game->o_level.event_count = 1;
  event *p_event = calloc(p_game->o_level.event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 1;
  p_event_param_warp->index_y = 8;
  p_event_param_warp->p_level_addr = level_overworld;

  event_init(p_event + 0, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp, 18, 7);

  p_game->o_level.p_event = p_event;
}
