#include "levels.h"

void NPC_show_dialog_on_action(NPC *p_NPC, game *p_game, SDL_Renderer *renderer)
{
  char *text = p_NPC->p_on_action_param;
  dialog_box_show(p_game, text, renderer);
}

// OVERWORLD
void level_overworld_NPC_0_on_action(NPC *p_NPC, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game, "YOU NEED THE CAVE KEY TO ENTER HERE", renderer);
}

void level_overworld(game *p_game, SDL_Renderer *renderer)
{
  level_free(p_game->p_level);
  level_load(p_game->p_level, "data/overworld.map", renderer);
  p_game->p_level->p_music = music_load(p_game->p_level->path_music);
  music_play(p_game->p_level->p_music);

  const int warp_count = 1;
  warp *p_warp = calloc(warp_count, sizeof(warp));
  p_warp->p_level_dest = level_cave;
  p_warp->index_src_x = 2;
  p_warp->index_src_y = 2;
  p_warp->index_x = 3;
  p_warp->index_y = 3;
  p_game->p_level->warp_cout = warp_count;
  p_game->p_level->p_warp = p_warp;

  int NPC_count = 1;
  NPC *p_NPC = calloc(NPC_count, sizeof(NPC));
  p_game->p_level->NPC_cout = NPC_count;
  p_game->p_level->p_NPC = p_NPC;

  for (int NPC_index = 0; NPC_index < p_game->p_level->NPC_cout; NPC_index++)
  {
    NPC_init(p_NPC + NPC_index, p_game->p_image_NPC, renderer);
  }

  char *text = "HELLO !";
  p_NPC[0].p_on_action_param = calloc(strlen(text) + 1, sizeof(char));
  strcpy(p_NPC[0].p_on_action_param, text);
  p_NPC[0].p_on_action_callback = NPC_show_dialog_on_action;
  p_NPC[0].p_sprite->x = 5 * p_game->p_level->p_map->tile_width;
  p_NPC[0].p_sprite->y = 6 * p_game->p_level->p_map->tile_height;
}

// CAVE
void level_cave(game *p_game, SDL_Renderer *renderer)
{
  level_free(p_game->p_level);
  level_load(p_game->p_level, "data/cave.map", renderer);
  p_game->p_level->p_music = music_load(p_game->p_level->path_music);
  music_play(p_game->p_level->p_music);

  const int warp_count = 1;
  warp *p_warp = calloc(warp_count, sizeof(warp));
  p_warp->p_level_dest = level_overworld;
  p_warp->index_src_x = 3;
  p_warp->index_src_y = 5;
  p_warp->index_x = 2;
  p_warp->index_y = 2;
  p_game->p_level->warp_cout = warp_count;
  p_game->p_level->p_warp = p_warp;
}
