#include <stdlib.h>
#include <time.h>
#include <getopt.h>

#include <SDL.h>

#include <init.h>
#include <image.h>
#include <fontmap.h>
#include <animation.h>

#include "../src/level.h"

#include "editor.h"
#include "level_util.h"

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

/**
   @return The next editor state
*/
editor_state state_in_editor(SDL_Renderer *renderer, editor *p_editor);

int main(int argc, char **argv)
{
  //flags for the window
  int flags = 0;

  //map to load path
  char *load_path = NULL;

  //parse command line arguments
  int c;
  while ((c = getopt(argc, argv, "hfi:")) != -1)
  {
    switch (c)
    {
    case 'h':
      printf("Tilemap display\n");
      printf("h: print this menu and exit\n");
      printf("f: launch in fullscreen\n");
      printf("i <filename>: input file to load\n");
      return EXIT_SUCCESS;
      break;
    case 'f':
      flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    case 'i':
      load_path = optarg;
      break;
    default:
      abort();
    }
  }

  init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  // create a new window
  SDL_Window *window = create_window("Tilemap Layout Editor",
                                     &WINDOW_WIDTH,
                                     &WINDOW_HEIGHT,
                                     flags);

  //create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                              0,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  //editor
  editor o_editor;
  editor_init(&o_editor);

  // fontmap
  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  image o_image_fontmap;
  image_load(&o_image_fontmap, "gfx/font.png", renderer, NULL);
  o_fontmap.p_image = &o_image_fontmap;
  o_editor.p_fontmap = &o_fontmap;

  //try to load a map from file
  if (load_path)
  {
    o_editor.path_level = load_path;

    //check file extension
    const char *extension = strrchr(load_path, '.');

    if (strcmp(extension, ".map") == 0)
    {
      map_init(&o_editor.o_level.o_tilemap);
      image o_image;
      o_image.p_texture = NULL;
      o_editor.o_level.o_tilemap.o_tileset.p_image = &o_image;
      if (!level_load(&o_editor.o_level, load_path, &o_editor.path_tileset, &o_editor.path_music, renderer))
      {
        printf("Cannot find map at %s\n", load_path);
        exit(0);
      }

      editor_state ret_code = IN_EDITOR;
      while (ret_code != QUIT)
      {
        switch (ret_code)
        {
        case IN_EDITOR:
          ret_code = editor_edit_layout(&o_editor, renderer);
          break;
        default:
          break;
        }
      }
    }
    else if (strcmp(extension, ".tileset") == 0)
    {
      tileset o_tileset;
      image o_image_tileset;
      o_image_tileset.p_texture = NULL;
      o_tileset.p_image = &o_image_tileset;
      tileset_init_from_file(&o_tileset, load_path, renderer);
      bool done = false;

      while (!done)
      {
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
              done = true;
              break;
            default:
              break;
            }
            break;
          }
        }

        //update animated tile
        for (int animation_index = 0; animation_index < o_tileset.animation_nb; animation_index++)
        {
          animation_update(o_tileset.v_animation + animation_index);
        }

        // update display
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        editor_render_tileset_animations(&o_tileset, renderer);
        SDL_RenderPresent(renderer);
      }
    }
    else
    {
      printf("Unknown file format\n");
      exit(0);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
  }
}