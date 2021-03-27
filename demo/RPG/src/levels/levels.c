#include "levels.h"

void level_overworld_event_callback(game *p_game, SDL_Renderer *renderer)
{
  printf("%s", "CALLBACK!\n");
}

void level_overworld(game *p_game, SDL_Renderer *renderer)
{
  level_load(&p_game->o_level, "data/overworld.map", &p_game->path_music, renderer);

  // events
  p_game->o_level.event_count = 7;
  event *p_event = calloc(p_game->o_level.event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 3;
  p_event_param_warp->index_y = 3;
  p_event_param_warp->p_level_addr = level_cave;
  p_event[0].p_param = p_event_param_warp;
  event_init(p_event + 0, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp, 2, 2);
  
  event_init(p_event + 1, ON_BUTTON_PRESS, EVENT_TYPE_TEXT, "TO TOWN", 1, 7);
  event_init(p_event + 2, ON_TILE_ENTER, EVENT_TYPE_FUNCTION, level_overworld_event_callback, 0, 5);
  event_init(p_event + 3, ON_BUTTON_PRESS, EVENT_TYPE_TEXT, "WELCOME TO THE RPG DEMO\nHAVE FUN!", -1, -1);
  event_init(p_event + 4, ON_BUTTON_PRESS, EVENT_TYPE_TEXT, "THERE IS SOMETHING IN THE\nWATER!", -1, -1);
  
  int *amount = malloc(sizeof(int));
  *amount = 30;
  event_init(p_event + 5, ON_BUTTON_PRESS, EVENT_TYPE_MONEY, amount, -1, -1);

  event_param_warp *p_event_param_warp_town = malloc(sizeof(event_param_warp));
  p_event_param_warp_town->index_x = 19;
  p_event_param_warp_town->index_y = 8;
  p_event_param_warp_town->p_level_addr = level_town;
  p_event[6].p_param = p_event_param_warp_town;
  event_init(p_event + 6, ON_TILE_ENTER, EVENT_TYPE_WARP, p_event_param_warp_town, 0, 8);

  p_game->o_level.p_event = p_event;

  // NPCs
  int NPC_count = 2;
  NPC *p_NPC = calloc(NPC_count, sizeof(NPC));
  p_game->o_level.NPC_count = NPC_count;
  p_game->o_level.p_NPC = p_NPC;

  for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
  {
    NPC_init(p_NPC + NPC_index, p_game->o_level.p_tileset + 1, renderer);
  }

  p_NPC[0].p_event = p_event + 3;
  p_NPC[0].o_sprite.x = 5 * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_NPC[0].o_sprite.y = 4 * p_game->o_level.o_tilemap.p_tileset->tile_height;

  p_NPC[1].p_event = p_event + 4;
  p_NPC[1].o_sprite.x = 15 * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_NPC[1].o_sprite.y = 10 * p_game->o_level.o_tilemap.p_tileset->tile_height;
  p_NPC[1].o_sprite.animation_current = LEFT;

  // chests
  p_game->o_level.chest_count = 2;
  chest *p_chest = calloc(p_game->o_level.chest_count, sizeof(chest));

  for (int chest_index = 0; chest_index < p_game->o_level.chest_count; chest_index++)
  {
    chest_init(p_chest + chest_index, p_game->p_tilesets + 1, renderer);
  }

  p_chest[0].id = 0;
  p_chest[0].p_event = p_event + 5;
  p_chest[0].o_sprite.x = 3 * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_chest[0].o_sprite.y = 3 * p_game->o_level.o_tilemap.p_tileset->tile_height;
  SDL_Rect bounding_box_0 = {.x = p_chest[0].o_sprite.x, .y = p_chest[0].o_sprite.y, .w = 16, .h = 16};
  p_chest[0].o_sprite.bounding_box = bounding_box_0;

  p_chest[1].id = 1;
  p_chest[1].p_event = p_event + 5;
  p_chest[1].o_sprite.x = 15 * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_chest[1].o_sprite.y = 13 * p_game->o_level.o_tilemap.p_tileset->tile_height;
  SDL_Rect bounding_box_1 = {.x = p_chest[1].o_sprite.x, .y = p_chest[1].o_sprite.y, .w = 16, .h = 16};
  p_chest[1].o_sprite.bounding_box = bounding_box_1;

  for (int chest_index = 0; chest_index < p_game->o_level.chest_count; chest_index++)
  {
    p_chest[chest_index].is_open = game_set_chest_is_open(p_game, p_chest + chest_index);
  }

  p_game->o_level.p_chest = p_chest;
}

void level_cave(game *p_game, SDL_Renderer *renderer)
{
  level_load(&p_game->o_level, "data/cave.map", &p_game->path_music, renderer);

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
  level_load(&p_game->o_level, "data/town.map", &p_game->path_music, renderer);

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
