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
#include "NPC.h"

#define SCREEN_WIDTH 15 * 16
#define SCREEN_HEIGHT 10 * 16

typedef void (*level_addr)(game *, SDL_Renderer *);

typedef struct t_game
{
  level *p_level;
  level_addr p_level_addr;
  hero *p_hero;
  tileset *p_tileset_NPC;
  fontmap *p_fontmap;
} game;

void game_draw(game *p_game, SDL_Renderer *renderer);
bool game_check_NPC_collid(game *p_game);
void game_center_camera_on_hero(game *p_game);
void game_update(game *p_game);

#endif
