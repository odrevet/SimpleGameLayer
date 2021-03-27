#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <init.h>
#include <fontmap.h>
#include <image.h>

#include "game.h"
#include "callbacks_user.h"

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

int main(int argc, char **argv)
{
  //flags for the window
  int flags = 0;

  game_state ret_code = MAIN_MENU;

  //parse command line arguments
  int c;
  while ((c = getopt(argc, argv, "hnf")) != -1)
  {
    switch (c)
    {
    case 'h':
      printf("Tilemap display\n");
      printf("h: print this menu and exit\n");
      printf("f: launch in fullscreen\n");
      printf("n: new game\n");
      return EXIT_SUCCESS;
      break;
    case 'f':
      flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    case 'n':
      ret_code = IN_GAME;
      break;
    default:
      abort();
    }
  }

  init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_ENABLE);

  SDL_Window *window = create_window("Point and Click Adventure",
                                         &WINDOW_WIDTH,
                                         &WINDOW_HEIGHT,
                                         flags);

  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              0,
                                              SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  //create a new game
  game o_game;
  o_game.has_key = false;

  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  o_fontmap.p_image = alloca(sizeof(image));
  image_load(o_fontmap.p_image, "res/font.png", renderer);
  o_game.p_fontmap = &o_fontmap;

  // assign user callbacks
  o_game.on_click[0] = callback_user_click_desk;
  o_game.on_click[1] = callback_user_click_bedroom_door;

  //game states
  while (ret_code != QUIT)
  {
    switch (ret_code)
    {
    case MAIN_MENU:
      ret_code = state_main_menu(renderer, &o_game);
      break;
    case IN_GAME:
      ret_code = state_in_game(renderer, &o_game);
      break;
    default:
      break;
    }
  }

  Mix_CloseAudio();
  SDL_Quit();
  return EXIT_SUCCESS;
}
