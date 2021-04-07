#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <init.h>
#include <image.h>
#include <tileset.h>
#include <sprite.h>
#include <tilemap.h>
#include <fontmap.h>

#include "level.h"
#include "hero.h"

#define SCREEN_WIDTH 15 * 16
#define SCREEN_HEIGHT 10 * 16

typedef struct t_game game;
typedef void (*level_addr)(game *, SDL_Renderer *);

typedef struct t_game
{
  level o_level;
  char *level_filepath;
  
  hero o_hero;

  fontmap *p_fontmap;

  int opened_chest_id_count;
  int *p_opened_chest_id;

  int tileset_count;
  char **path_tilesets;
  tileset *p_tilesets;

  char *path_music;
} game;

void game_draw(game *p_game, SDL_Renderer *renderer);
bool game_get_chest_is_open(game *p_game, int chest_id);
void game_center_camera_on_hero(game *p_game);
void game_update(game *p_game);
void event_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
void event_money_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
void game_free(game *p_game);
#endif
