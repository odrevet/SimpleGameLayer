#include "init.h"

void init(int flags)
{
  if (flags == 0)
    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
  /*initialize SDL video*/
  if (SDL_Init(flags) < 0)
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

SDL_Window *create_window(char *name, int *width, int *height, int flags)
{
  SDL_Window *window = SDL_CreateWindow(name,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        *width, *height,
                                        flags);

  // Declare display mode structure to be filled in.
  SDL_DisplayMode dm;

  for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
  {

    int video_display_num = SDL_GetCurrentDisplayMode(i, &dm);

    if (video_display_num != 0)
    {
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s",
              video_display_num,
              SDL_GetError());
    }
  }

  bool fullscreen = false;
  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
    fullscreen = true;

  if (fullscreen)
  {
    *width = dm.w;
    *height = dm.h;
  }

  return window;
}
