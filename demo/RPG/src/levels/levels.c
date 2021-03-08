#include "levels.h"

// OVERWORLD
void level_overworld_event_callback(game *p_game, SDL_Renderer *renderer)
{
  printf("%s", "CALLBACK!\n");
}

void level_overworld(game *p_game, SDL_Renderer *renderer)
{
  level_load(&p_game->o_level, "data/overworld.map", &p_game->path_tileset, &p_game->path_music, renderer);

  // events
  const int event_count = 5;
  event *p_event = calloc(event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 3;
  p_event_param_warp->index_y = 3;
  p_event_param_warp->p_level_addr = level_cave;
  p_event[0].p_param = p_event_param_warp;
  p_event[0].o_event_trigger = ON_TILE_ENTER;
  p_event[0].o_event_type = EVENT_TYPE_WARP;
  p_event[0].has_triggered = false;
  p_event[0].index_src_x = 2;
  p_event[0].index_src_y = 2;

  p_event[1].p_param = "TO TOWN";
  p_event[1].o_event_trigger = ON_BUTTON_PRESS;
  p_event[1].o_event_type = EVENT_TYPE_TEXT;
  p_event[1].has_triggered = false;
  p_event[1].index_src_x = 1;
  p_event[1].index_src_y = 7;

  p_event[2].p_param = level_overworld_event_callback;
  p_event[2].o_event_trigger = ON_TILE_ENTER;
  p_event[2].o_event_type = EVENT_TYPE_FUNCTION;
  p_event[2].has_triggered = false;
  p_event[2].index_src_x = 0;
  p_event[2].index_src_y = 5;

  p_event[3].p_param = "WELCOME TO THE RPG DEMO\nHAVE FUN!";
  p_event[3].o_event_trigger = ON_BUTTON_PRESS;
  p_event[3].o_event_type = EVENT_TYPE_TEXT;
  p_event[3].has_triggered = false;

  p_event[4].p_param = "THERE IS SOMETHING IN THE\nWATER!";
  p_event[4].o_event_trigger = ON_BUTTON_PRESS;
  p_event[4].o_event_type = EVENT_TYPE_TEXT;
  p_event[4].has_triggered = false;

  p_game->o_level.event_count = event_count;
  p_game->o_level.p_event = p_event;

  // NPCs
  int NPC_count = 2;
  NPC *p_NPC = calloc(NPC_count, sizeof(NPC));
  p_game->o_level.NPC_count = NPC_count;
  p_game->o_level.p_NPC = p_NPC;

  for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
  {
    NPC_init(p_NPC + NPC_index, p_game->p_tileset_NPC, renderer);
  }

  p_NPC[0].p_event = p_event + 3;
  p_NPC[0].o_sprite.x = 5 * p_game->o_level.o_tilemap.o_tileset.tile_width;
  p_NPC[0].o_sprite.y = 4 * p_game->o_level.o_tilemap.o_tileset.tile_height;

  p_NPC[1].p_event = p_event + 4;
  p_NPC[1].o_sprite.x = 15 * p_game->o_level.o_tilemap.o_tileset.tile_width;
  p_NPC[1].o_sprite.y = 10 * p_game->o_level.o_tilemap.o_tileset.tile_height;
  p_NPC[1].o_sprite.animation_current = LEFT;
}

// CAVE
void level_cave(game *p_game, SDL_Renderer *renderer)
{
  level_load(&p_game->o_level, "data/cave.map", &p_game->path_tileset, &p_game->path_music, renderer);

  // Events
  const int event_count = 1;
  event *p_event = calloc(event_count, sizeof(event));

  event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
  p_event_param_warp->index_x = 2;
  p_event_param_warp->index_y = 2;
  p_event_param_warp->p_level_addr = level_overworld;
  p_event[0].p_param = p_event_param_warp;
  p_event[0].o_event_trigger = ON_TILE_ENTER;
  p_event[0].o_event_type = EVENT_TYPE_WARP;
  p_event[0].has_triggered = false;
  p_event[0].index_src_x = 3;
  p_event[0].index_src_y = 5;
  p_game->o_level.event_count = event_count;
  p_game->o_level.p_event = p_event;
}
