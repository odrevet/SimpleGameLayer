#include "levels.h"

void level_overworld(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/overworld.map", renderer);
  level_init_from_file(&p_game->o_level, "data/overworld.level", &p_game->path_music, renderer);

  for (int chest_index = 0; chest_index < p_game->o_level.chest_count; chest_index++)
  {
    chest_init(p_game->o_level.p_chest + chest_index, p_game->p_tilesets + 1, renderer);
    p_game->o_level.p_chest[chest_index].is_open = game_get_chest_is_open(p_game, p_game->o_level.p_chest[chest_index].id);
    SDL_Rect bounding_box = {.x = p_game->o_level.p_chest[chest_index].o_sprite.x, .y = p_game->o_level.p_chest[chest_index].o_sprite.y, .w = 16, .h = 16};
    p_game->o_level.p_chest[chest_index].o_sprite.bounding_box = bounding_box;
  }

  for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
  {
    NPC_init(p_game->o_level.p_NPC + NPC_index, p_game->o_level.p_tileset + 0, renderer);
  }
}

void level_cave(game *p_game, SDL_Renderer *renderer)
{
  level_map_init_from_file(&p_game->o_level, "data/cave.map", renderer);

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
