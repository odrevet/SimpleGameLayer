#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include <SDL.h>

#include <init.h>
#include <fontmap.h>
#include <image.h>
#include <UI/menu.h>

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;
int SCREEN_WIDTH = 320;
int SCREEN_HEIGHT = 240;

int menu_run(menu *p_menu, SDL_Renderer *renderer, fontmap *p_fontmap, char *cursor, void *param)
{
  int ret_code;
  bool menu_done = false;

  while (!menu_done)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_RETURN:
          ret_code = (p_menu->p_entries + p_menu->index_choice)->on_action(param, p_menu->p_entries[p_menu->index_choice].param, renderer);
          menu_done = true;
          break;
        case SDLK_UP:
          menu_index_dec(p_menu);
          break;
        case SDLK_DOWN:
          menu_index_inc(p_menu);
          break;
        }
      }
    }
    menu_draw(p_menu, renderer, p_fontmap, cursor, param);
  }
  return ret_code;
}

int menu_entry_one_callback(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  int *param_actual = (int *)param_entry;
  printf("menu entry one callback with param %d\n", *param_actual);
  return 0;
}

int menu_entry_two_callback(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  printf("menu entry two callback\n");
  return 0;
}

void menu_set_default_style(menu *p_menu)
{
  p_menu->o_style.padding = 10;
  p_menu->o_style.x = SCREEN_WIDTH / 2;
  p_menu->o_style.y = SCREEN_HEIGHT / 2;
  SDL_Color border = {.r = 200, .g = 200, .b = 200, .a = 255};
  p_menu->o_style.border = border;
  SDL_Color bgcolor = {.r = 100, .g = 100, .b = 100, .a = 200};
  p_menu->o_style.bgcolor = bgcolor;
}

int main(int argc, char **argv)
{
  int window_flags = 0;

  //parse command line arguments
  int c;
  while ((c = getopt(argc, argv, "hfm:")) != -1)
  {
    switch (c)
    {
    case 'h':
      printf("Menu Demo\n");
      printf("h: print this menu and exit\n");
      printf("f: launch in fullscreen\n");
      return EXIT_SUCCESS;
      break;
    case 'f':
      window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    default:
      abort();
    }
  }

  init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_DISABLE);

  SDL_Window *window = create_window("Menu Demo",
                                     &WINDOW_WIDTH,
                                     &WINDOW_HEIGHT,
                                     window_flags);

  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              0,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  int param = 42;

  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  image o_image;
  image_load(&o_image, "res/font.png", renderer, NULL);
  o_fontmap.p_image = &o_image;

  menu o_menu;
  menu_init(&o_menu);
  menu_set_default_style(&o_menu);
  o_menu.o_style.x = 50;
  o_menu.o_style.y = 50;
  o_menu.o_style.padding = 10;

  menu_add_entry(&o_menu, "ENTRY 1", menu_entry_one_callback, &param);
  menu_add_entry(&o_menu, "ENTRY 2", menu_entry_two_callback, NULL);

  menu_run(&o_menu, renderer, &o_fontmap, "-", NULL);

  menu_free(&o_menu);

  return EXIT_SUCCESS;
}
