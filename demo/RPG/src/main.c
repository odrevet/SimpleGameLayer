#include <getopt.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <UI/dialog.h>
#include <animation.h>
#include <fontmap.h>
#include <image.h>
#include <init.h>
#include <sound.h>

#include "NPC.h"
#include "dialog_box.h"
#include "game.h"
#include "hero.h"
#include "level.h"
#include "levels/levels.h"
#include "warp.h"

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

typedef enum e_game_state
{
  STAY = -1,
  QUIT,
  IN_GAME,
} game_state;

/**
   @return The next game state
*/
game_state state_in_game(game *p_game, SDL_Renderer *renderer);

int main(int argc, char **argv)
{
  // flags for the window
  int flags = 0;

  // parse command line arguments
  int c;
  while ((c = getopt(argc, argv, "hf")) != -1)
  {
    switch (c)
    {
    case 'h':
      printf("RPG\n");
      printf("h: print this menu and exit\n");
      printf("f: launch in fullscreen\n");
      return EXIT_SUCCESS;
      break;
    case 'f':
      flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    default:
      abort();
    }
  }

  init(SDL_INIT_VIDEO);
  sound_init();
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_DISABLE);

  // create a new window
  SDL_Window *window =
      create_window("RPG", &WINDOW_WIDTH, &WINDOW_HEIGHT, flags);

  // create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  // create a new game
  game o_game;

  tilemap o_tilemap;
  map_init(&o_tilemap);

  level o_level;
  level_init(&o_level);

  image o_image_tilemap;
  o_game.p_level = &o_level;
  o_game.p_level->p_map = &o_tilemap;
  o_game.p_level->p_map->p_image = &o_image_tilemap;

  image o_image_NPC;
  image_load(&o_image_NPC, "gfx/NPC_test.png", renderer, NULL);
  o_game.p_image_NPC = &o_image_NPC;

  // hero
  hero o_hero;
  o_game.p_hero = &o_hero;
  hero_init(o_game.p_hero, renderer);

  // fontmap
  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  image o_image_fontmap;
  image_load(&o_image_fontmap, "gfx/font.png", renderer, NULL);
  o_fontmap.p_image = &o_image_fontmap;
  o_game.p_fontmap = &o_fontmap;

  // load the initial level
  level_overworld(&o_game, renderer);

  // game states
  game_state ret_code = IN_GAME;
  while (ret_code != QUIT)
  {
    switch (ret_code)
    {
    case IN_GAME:
      ret_code = state_in_game(&o_game, renderer);
      break;
    default:
      break;
    }
  }

  // free memory
  level_free(o_game.p_level);
  Mix_CloseAudio();

  // free images
  image_free(&o_image_fontmap);
  image_free(&o_image_NPC);

  SDL_Quit();
  return EXIT_SUCCESS;
}

void game_check_warp(game *p_game, int hero_current_tile_index_x,
                     int hero_current_tile_index_y, SDL_Renderer *renderer)
{
  for (int index_warp = 0; index_warp < p_game->p_level->warp_cout;
       index_warp++)
  {
    warp *p_warp = p_game->p_level->p_warp + index_warp;
    if (p_warp->index_src_x == hero_current_tile_index_x &&
        p_warp->index_src_y == hero_current_tile_index_y)
    {
      warp_exec(p_warp, p_game, renderer);
    }
  }
}

void game_check_NPC_action(game *p_game, int hero_center_x, int hero_center_y,
                           SDL_Renderer *renderer)
{
  int hero_direction = p_game->p_hero->p_sprite->p_tileset->animation_current;
  int hero_front_x = hero_center_x;
  int hero_front_y = hero_center_y;

  switch (hero_direction)
  {
  case UP:
    hero_front_y -= 8;
    break;
  case RIGHT:
    hero_front_x += 12;
    break;
  case DOWN:
    hero_front_y += 8;
    break;
  case LEFT:
    hero_front_x -= 12;
    break;
  }

  if (hero_front_x >= 0 && hero_front_x < p_game->p_level->p_map->width * p_game->p_level->p_map->tile_width &&
      hero_front_y >= 0 && hero_front_y < p_game->p_level->p_map->height * p_game->p_level->p_map->tile_height)
  {
    for (int index_NPC = 0; index_NPC < p_game->p_level->NPC_cout;
         index_NPC++)
    {
      NPC *p_NPC = p_game->p_level->p_NPC + index_NPC;
      SDL_Point hero_front = {.x = hero_front_x, .y = hero_front_y};
      if (SDL_PointInRect(&hero_front, &p_NPC->p_sprite->bounding_box))
      {
        p_NPC->p_on_action_callback(p_NPC, p_game, renderer);
        break;
      }
    }
  }
}

game_state state_in_game(game *p_game, SDL_Renderer *renderer)
{
  bool done = false;

  game_state ret_code = QUIT;
  tilemap *p_map = p_game->p_level->p_map;

  while (!done)
  {
    // update map tile animation
    /*if (p_game->nb_tile > 0)
    {
      for (int i = 0; i < p_game->nb_tile; i++)
      {
        animation_update(p_game->v_tile[i].p_animation);
      }
    }*/

    // hero current tile
    int hero_center_x = p_game->p_hero->p_sprite->bounding_box.x +
                        p_game->p_hero->p_sprite->bounding_box.w / 2;
    int hero_center_y = p_game->p_hero->p_sprite->bounding_box.y +
                        p_game->p_hero->p_sprite->bounding_box.h / 2;
    int hero_current_tile_index_x = hero_center_x / p_map->tile_width;
    int hero_current_tile_index_y = hero_center_y / p_map->tile_height;

    // input
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

      if (event.type == SDL_QUIT)
      {
        exit(EXIT_SUCCESS);
      }

      if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
      {
        switch (event.key.keysym.sym)
        {
        case SDLK_RETURN:
          game_check_NPC_action(p_game, hero_center_x, hero_center_y, renderer);
        }
      }
    }

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // quit
    if (state[SDL_GetScancodeFromKey(SDLK_q)])
    {
      done = true;
      ret_code = QUIT;
      break;
    }

    if (state[SDL_SCANCODE_LEFT])
    {
      p_game->p_hero->p_sprite->p_tileset->animation_current = LEFT;
      p_game->p_hero->p_sprite->vel_y = 0;
      p_game->p_hero->p_sprite->vel_x = -1;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
      p_game->p_hero->p_sprite->p_tileset->animation_current = RIGHT;
      p_game->p_hero->p_sprite->vel_y = 0;
      p_game->p_hero->p_sprite->vel_x = 1;
    }
    else if (state[SDL_SCANCODE_UP])
    {
      p_game->p_hero->p_sprite->p_tileset->animation_current = UP;
      p_game->p_hero->p_sprite->vel_y = -1;
      p_game->p_hero->p_sprite->vel_x = 0;
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
      p_game->p_hero->p_sprite->p_tileset->animation_current = DOWN;
      p_game->p_hero->p_sprite->vel_y = 1;
      p_game->p_hero->p_sprite->vel_x = 0;
    }
    else
    {
      p_game->p_hero->p_sprite->vel_x = 0;
      p_game->p_hero->p_sprite->vel_y = 0;
    }

    // update logic
    game_update(p_game);
    // check if the hero walk on a warp
    game_check_warp(p_game, hero_current_tile_index_x, hero_current_tile_index_y, renderer);

    // update display
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    game_draw(p_game, renderer);
    SDL_RenderPresent(renderer);
  }

  return ret_code;
}
