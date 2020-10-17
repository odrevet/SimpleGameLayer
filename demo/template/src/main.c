#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include <SDL.h>

#include <init.h>

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 240;

#define APP_NAME "template"

typedef enum e_game_state
{
  STAY = -1,
  QUIT,
  IN_GAME,
} game_state;

typedef struct t_game
{
} game;

game_state state_in_game(SDL_Renderer *renderer, game *p_game);

int main(int argc, char **argv)
{
  int window_flags = 0;

  //parse command line arguments
  int c;
  extern char *optarg;
  while ((c = getopt(argc, argv, "hf")) != -1)
    switch (c)
    {
    case 'h':
      printf("%s\nOptions\n-f : fullscreen", APP_NAME);
      break;
    case 'f':
      window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    default:
      abort();
    }

  init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_DISABLE);

  // create a new window
  SDL_Window *window = create_window(APP_NAME,
                                     &WINDOW_WIDTH,
                                     &WINDOW_HEIGHT,
                                     window_flags);
  //create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              0,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  //create a new game
  game o_game;

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
    //draw
    //background
    SDL_SetRenderDrawColor(renderer, 24, 42, 0, 255);
    SDL_RenderClear(renderer);

    //input
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

      if (event.type == SDL_QUIT)
      {
        exit(EXIT_SUCCESS);
      }

      switch (event.type)
      {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_q:
          exit(EXIT_SUCCESS);
          break;
        default:
          break;
        }
        break;
      case SDL_JOYBUTTONDOWN:
        switch (event.jbutton.button)
        {
        }
        break;
      }
    }

    //update display
    SDL_RenderPresent(renderer);
  }

  return ret_code;
}
