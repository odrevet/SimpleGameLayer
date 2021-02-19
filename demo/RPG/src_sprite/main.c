#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include <init.h>
#include <image.h>
#include <sprite.h>

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 240;

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

typedef enum e_game_state
{
  STAY = -1,
  QUIT,
  IN_GAME,
} game_state;

typedef struct t_game
{
  sprite *p_sprite;
} game;

game_state state_in_game(SDL_Renderer *renderer, game *p_game);

int main(int argc, char **argv)
{
  int flags = 0;

  int c;
  while ((c = getopt(argc, argv, "f")) != -1)
  {
    switch (c)
    {
    case 'f':
      flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    default:
      abort();
    }
  }

  init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_DISABLE);

  // create a new window
  SDL_Window *window = create_window("Sprite user Input",
                                     &WINDOW_WIDTH,
                                     &WINDOW_HEIGHT,
                                     flags);
  //create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              0,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  game o_game;

  tileset o_tileset;
  o_tileset.p_image = malloc(sizeof(image));
  image_load(o_tileset.p_image, "gfx/NPC_test.png", renderer, NULL);
  tileset_init(&o_tileset, 4, 1, 4);

  sprite o_sprite;
  o_sprite.x = SCREEN_WIDTH / 2;
  o_sprite.y = SCREEN_HEIGHT / 2;
  o_sprite.vel_x = 0;
  o_sprite.vel_y = 0;
  o_sprite.animation_current = DOWN;
  o_sprite.p_tileset = &o_tileset;

  o_game.p_sprite = &o_sprite;

  animation_init(o_tileset.v_animation + DOWN, false, 4, 10, 0);
  tileset_set_frame(&o_tileset, DOWN, 0, 00, 0, 14, 21);
  tileset_set_frame(&o_tileset, DOWN, 1, 14, 0, 14, 21);
  tileset_set_frame(&o_tileset, DOWN, 2, 00, 0, 14, 21);
  tileset_set_frame(&o_tileset, DOWN, 3, 28, 0, 14, 21);

  animation_init(o_tileset.v_animation + UP, false, 4, 10, 0);
  tileset_set_frame(&o_tileset, UP, 0, 00, 42, 14, 21);
  tileset_set_frame(&o_tileset, UP, 1, 14, 42, 14, 21);
  tileset_set_frame(&o_tileset, UP, 2, 00, 42, 14, 21);
  tileset_set_frame(&o_tileset, UP, 3, 28, 42, 14, 21);

  animation_init(o_tileset.v_animation + LEFT, false, 4, 10, 0);
  tileset_set_frame(&o_tileset, LEFT, 0, 00, 64, 14, 21);
  tileset_set_frame(&o_tileset, LEFT, 1, 14, 64, 14, 21);
  tileset_set_frame(&o_tileset, LEFT, 2, 00, 63, 14, 21);
  tileset_set_frame(&o_tileset, LEFT, 3, 28, 64, 14, 21);

  animation_init(o_tileset.v_animation + RIGHT, false, 4, 10, 0);
  tileset_set_frame(&o_tileset, RIGHT, 0, 00, 21, 14, 21);
  tileset_set_frame(&o_tileset, RIGHT, 1, 14, 21, 14, 21);
  tileset_set_frame(&o_tileset, RIGHT, 2, 00, 21, 14, 21);
  tileset_set_frame(&o_tileset, RIGHT, 3, 28, 21, 14, 21);

  //game stats
  game_state ret_code = IN_GAME;
  while (ret_code != QUIT)
  {
    switch (ret_code)
    {
    case IN_GAME:
      ret_code = state_in_game(renderer, &o_game);
      break;
    default:
      break;
    }
  }

  SDL_Quit();
  return EXIT_SUCCESS;
}

game_state state_in_game(SDL_Renderer *renderer, game *p_game)
{
  bool done = false;
  game_state ret_code = QUIT;

  while (!done)
  {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    sprite_draw(p_game->p_sprite, renderer);

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //quit
    if (state[SDL_GetScancodeFromKey(SDLK_q)])
    {
      done = true;
      ret_code = QUIT;
      break;
    }

    if (state[SDL_SCANCODE_LEFT])
    {
      p_game->p_sprite->animation_current = LEFT;
      p_game->p_sprite->vel_y = 0;
      p_game->p_sprite->vel_x = -1;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
      p_game->p_sprite->animation_current = RIGHT;
      p_game->p_sprite->vel_y = 0;
      p_game->p_sprite->vel_x = 1;
    }
    else if (state[SDL_SCANCODE_UP])
    {
      p_game->p_sprite->animation_current = UP;
      p_game->p_sprite->vel_y = -1;
      p_game->p_sprite->vel_x = 0;
    }
    else if (state[SDL_SCANCODE_DOWN])
    {
      p_game->p_sprite->animation_current = DOWN;
      p_game->p_sprite->vel_y = 1;
      p_game->p_sprite->vel_x = 0;
    }
    else
    {
      p_game->p_sprite->vel_x = 0;
      p_game->p_sprite->vel_y = 0;
    }

    p_game->p_sprite->x += p_game->p_sprite->vel_x;
    p_game->p_sprite->y += p_game->p_sprite->vel_y;
    sprite_update_frame(p_game->p_sprite);

    SDL_RenderPresent(renderer);
  }

  return ret_code;
}
