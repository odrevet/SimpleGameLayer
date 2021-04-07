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

#include "dialog_box.h"
#include "event.h"
#include "utils.h"
#include "NPC.h"
#include "level.h"
#include "hero.h"
#include "game.h"
#include "levels/levels.h"

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
  SDL_Window *window = create_window("RPG", &WINDOW_WIDTH, &WINDOW_HEIGHT, flags);

  // create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  // create a new game
  game o_game;
  level_init(&o_game.o_level);
  map_init(&o_game.o_level.o_tilemap);
  o_game.o_level.tileset_count = 0;
  o_game.o_level.p_tileset = NULL;
  o_game.o_level.p_tileset_path = NULL;
  o_game.o_level.p_tilemap_tileset_path = NULL;
  o_game.path_music = NULL;
  o_game.opened_chest_id_count = 0;
  o_game.p_opened_chest_id = NULL;

  o_game.o_level.callbacks[0] = NULL;
  o_game.o_level.callbacks[1] = level_cave;
  o_game.o_level.callbacks[2] = level_town;
  o_game.o_level.callbacks[3] = NULL;

  // load game's tilesets
  o_game.tileset_count = 2;
  o_game.p_tilesets = calloc(o_game.tileset_count, sizeof(tileset));
  o_game.path_tilesets = calloc(o_game.tileset_count, sizeof(char *));

  o_game.path_tilesets[0] = calloc(strlen("data/hero.tileset") + 1, sizeof(char));
  strcpy(o_game.path_tilesets[0], "data/hero.tileset");
  tileset_init_from_file(o_game.p_tilesets + 0, o_game.path_tilesets[0], renderer);

  o_game.path_tilesets[1] = calloc(strlen("data/objects.tileset") + 1, sizeof(char));
  strcpy(o_game.path_tilesets[1], "data/objects.tileset");
  tileset_init_from_file(o_game.p_tilesets + 1, o_game.path_tilesets[1], renderer);

  // hero
  hero_init(&o_game.o_hero, renderer);
  o_game.o_hero.o_sprite.p_tileset = o_game.p_tilesets + 0;

  // fontmap
  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  image o_image_fontmap;
  image_load(&o_image_fontmap, "gfx/font.png", renderer);
  o_fontmap.p_image = &o_image_fontmap;
  o_game.p_fontmap = &o_fontmap;

  // load the initial level by exec a warp event
  event o_initial_warp;
  event_param_warp o_initial_warp_param;
  o_initial_warp.p_param = &o_initial_warp_param;

  FILE *fp = fopen("data/init", "r");
  if (!fp)
  {
    printf("Cannot found init file");
    exit(0);
  }

  char buffer[256];
  fscanf(fp, "%d %d %s", &o_initial_warp_param.index_x, &o_initial_warp_param.index_y, buffer);
  o_initial_warp_param.p_level_filepath = buffer;
  event_warp_exec(&o_initial_warp, &o_game, renderer);

  fclose(fp);

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
  for (int event_index = 0; event_index < o_game.o_level.event_count; event_index++)
  {
    event_free(o_game.o_level.p_event + event_index);
  }
  game_free(&o_game);
  Mix_CloseAudio();

  SDL_Quit();
  return EXIT_SUCCESS;
}

void game_check_auto_event(game *p_game, int hero_current_tile_index_x, int hero_current_tile_index_y, SDL_Renderer *renderer)
{
  for (int index_event = 0; index_event < p_game->o_level.event_count;
       index_event++)
  {
    event *p_event = p_game->o_level.p_event + index_event;
    if (p_event->index_src_x == hero_current_tile_index_x &&
        p_event->index_src_y == hero_current_tile_index_y &&
        p_event->o_event_trigger == ON_TILE_ENTER &&
        !p_event->has_triggered)
    {
      event_exec(p_event, p_game, renderer);
      break;
    }
  }
}

void game_check_on_bouton_press_event(game *p_game, int hero_center_x, int hero_center_y, SDL_Renderer *renderer)
{
  int hero_front_x = hero_center_x;
  int hero_front_y = hero_center_y;

  switch (p_game->o_hero.o_sprite.animation_current)
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

  if (hero_front_x >= 0 && hero_front_x < p_game->o_level.o_tilemap.width * p_game->o_level.o_tilemap.p_tileset->tile_width &&
      hero_front_y >= 0 && hero_front_y < p_game->o_level.o_tilemap.height * p_game->o_level.o_tilemap.p_tileset->tile_height)
  {
    SDL_Point hero_front = {.x = hero_front_x, .y = hero_front_y};

    // NPCs
    for (int index_NPC = 0; index_NPC < p_game->o_level.NPC_count; index_NPC++)
    {
      NPC *p_NPC = p_game->o_level.p_NPC + index_NPC;
      if (SDL_PointInRect(&hero_front, &p_NPC->o_sprite.bounding_box) && p_NPC->p_event)
      {
        event_exec(p_NPC->p_event, p_game, renderer);
        break;
      }
    }

    // chests
    for (int index_chest = 0; index_chest < p_game->o_level.chest_count; index_chest++)
    {
      chest *p_chest = p_game->o_level.p_chest + index_chest;
      if (SDL_PointInRect(&hero_front, &p_chest->o_sprite.bounding_box) && p_chest->p_event && !p_chest->is_open)
      {
        p_chest->is_open = true;
        p_game->opened_chest_id_count++;
        p_game->p_opened_chest_id = realloc(p_game->p_opened_chest_id, p_game->opened_chest_id_count * sizeof(int));
        p_game->p_opened_chest_id[p_game->opened_chest_id_count - 1] = p_chest->id;

        event_exec(p_chest->p_event, p_game, renderer);
        break;
      }
    }

    // level events
    for (int index_event = 0; index_event < p_game->o_level.event_count; index_event++)
    {
      event *p_event = p_game->o_level.p_event + index_event;
      if (p_event->o_event_trigger != ON_BUTTON_PRESS)
      {
        continue;
      }
      SDL_Point hero_front = {.x = hero_front_x, .y = hero_front_y};
      SDL_Rect event_rect = {.x = p_event->index_src_x * 16,
                             .y = p_event->index_src_y * 16,
                             .w = p_game->o_level.o_tilemap.p_tileset->tile_width,
                             .h = p_game->o_level.o_tilemap.p_tileset->tile_height};
      if (SDL_PointInRect(&hero_front, &event_rect))
      {
        event_exec(p_event, p_game, renderer);
        break;
      }
    }
  }
}

game_state state_in_game(game *p_game, SDL_Renderer *renderer)
{
  bool done = false;

  game_state ret_code = QUIT;
  tilemap *p_map = &p_game->o_level.o_tilemap;

  while (!done)
  {
    // hero current tile
    int hero_center_x = p_game->o_hero.o_sprite.bounding_box.x +
                        p_game->o_hero.o_sprite.bounding_box.w / 2;
    int hero_center_y = p_game->o_hero.o_sprite.bounding_box.y +
                        p_game->o_hero.o_sprite.bounding_box.h / 2;
    int hero_current_tile_index_x = hero_center_x / p_map->p_tileset->tile_width;
    int hero_current_tile_index_y = hero_center_y / p_map->p_tileset->tile_height;

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
          game_check_on_bouton_press_event(p_game, hero_center_x, hero_center_y, renderer);
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
      p_game->o_hero.o_sprite.animation_current = LEFT;
      p_game->o_hero.o_sprite.vel_y = 0;
      p_game->o_hero.o_sprite.vel_x = -1;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
      p_game->o_hero.o_sprite.animation_current = RIGHT;
      p_game->o_hero.o_sprite.vel_y = 0;
      p_game->o_hero.o_sprite.vel_x = 1;
    }
    else if (state[SDL_SCANCODE_UP])
    {
      p_game->o_hero.o_sprite.animation_current = UP;
      p_game->o_hero.o_sprite.vel_y = -1;
      p_game->o_hero.o_sprite.vel_x = 0;
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
      p_game->o_hero.o_sprite.animation_current = DOWN;
      p_game->o_hero.o_sprite.vel_y = 1;
      p_game->o_hero.o_sprite.vel_x = 0;
    }
    else
    {
      p_game->o_hero.o_sprite.vel_x = 0;
      p_game->o_hero.o_sprite.vel_y = 0;
    }

    // update logic
    game_update(p_game);
    game_check_auto_event(p_game, hero_current_tile_index_x, hero_current_tile_index_y, renderer);

    // update display
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    game_draw(p_game, renderer);
    SDL_RenderPresent(renderer);
  }

  return ret_code;
}
