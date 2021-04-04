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
  image_load(&o_image_fontmap, "gfx/font.png", renderer);
  o_fontmap.p_image = &o_image_fontmap;
  o_editor.p_fontmap = &o_fontmap;
  level_init(&o_editor.o_level);
   o_editor.o_level.p_tilemap_tileset_path = NULL;

  //try to load a map from file
  if (load_path)
  {
    o_editor.path_level = load_path;

    //check file extension
    const char *extension = strrchr(load_path, '.');

    if (strcmp(extension, ".map") == 0)
    {
      map_init(&o_editor.o_level.o_tilemap);

      if (!level_map_init_from_file(&o_editor.o_level, load_path, renderer))
      {
        printf("Cannot find map at %s\n", load_path);
        exit(0);
      }

      o_editor.p_tileset = o_editor.o_level.o_tilemap.p_tileset;

      editor_state ret_code = LAYOUT_EDITOR;
      while (ret_code != QUIT)
      {
        switch (ret_code)
        {
        case LAYOUT_EDITOR:
          ret_code = editor_edit_layout(&o_editor, renderer);
          break;
        default:
          break;
        }
      }
    }
    else if (strcmp(extension, ".tileset") == 0)
    {
      o_editor.p_tileset = alloca(sizeof(tileset));
      tileset_init_from_file(o_editor.p_tileset, load_path, renderer);
      editor_tile_selection(&o_editor, renderer);

      editor_state ret_code = ANIMATED_TILE_SELECTION;
      while (ret_code != QUIT)
      {
        switch (ret_code)
        {
        case TILE_SELECTION:
          ret_code = editor_edit_layout(&o_editor, renderer);
          break;
        case ANIMATED_TILE_SELECTION:
          ret_code = editor_animated_tile_selection(&o_editor, renderer);
          break;
        default:
          break;
        }
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