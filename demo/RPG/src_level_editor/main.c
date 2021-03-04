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
  char *level_load_path = NULL;

  //parse command line arguments
  int c;
  while ((c = getopt(argc, argv, "hfl:")) != -1)
  {
    switch (c)
    {
    case 'h':
      printf("Tilemap display\n");
      printf("h: print this menu and exit\n");
      printf("f: launch in fullscreen\n");
      printf("l <filename>: path of the level to load (default %s)\n", level_load_path);
      return EXIT_SUCCESS;
      break;
    case 'f':
      flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
      break;
    case 'l':
      level_load_path = optarg;
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

  //tilemap
  tilemap o_tilemap;
  o_tilemap.p_tileset = malloc(sizeof(tileset));
  map_init(&o_tilemap);

  //image
  image o_image;
  o_image.p_texture = NULL;
  
  //level
  level o_level;
  level_init(&o_level);
  o_editor.p_level = &o_level;
  o_editor.p_level->p_map = &o_tilemap;
  o_editor.p_level->p_map->p_tileset->p_image = &o_image;
  

  // fontmap
  fontmap o_fontmap;
  o_fontmap.character_size = 7;
  o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
  image o_image_fontmap;
  image_load(&o_image_fontmap, "gfx/font.png", renderer, NULL);
  o_fontmap.p_image = &o_image_fontmap;
  o_editor.p_fontmap = &o_fontmap;

  //try to load a map from file
  if (level_load_path)
  {
    //check file extension to known which maploader use
    const char *extension = strrchr(level_load_path, '.');

    if (strcmp(extension, ".map") == 0)
    {
      if (!level_load(o_editor.p_level, level_load_path, &o_editor.path_tileset, &o_editor.path_music, renderer))
      {
        printf("Cannot find map at %s\n", level_load_path);
        printf("Create it y/n ? \n");
        char chr;
        scanf("%c", &chr);
        if (chr != 'y')
        {
          exit(EXIT_SUCCESS);
        }

        //create a new map from user input
        level *p_level = o_editor.p_level;
        char path_tileset[256];

        printf("Creation of a new map\n");
        printf("Map Layers count (in tile) : \n");
        scanf("%d", &p_level->p_map->nb_layer);
        printf("Tile Width (in pixel) : \n");
        scanf("%d", &p_level->p_map->p_tileset->tile_width);
        printf("Tile Height (in pixel) : \n");
        scanf("%d", &p_level->p_map->p_tileset->tile_height);
        printf("Map Width (in tile) : \n");
        scanf("%d", &p_level->p_map->width);
        printf("Map Height (in tile): \n");
        scanf("%d", &p_level->p_map->height);
        printf("Tilset image (path) : \n");
        scanf("%s", path_tileset);
        o_editor.path_tileset = calloc(strlen(path_tileset) + 1, sizeof(char));
        strcpy(o_editor.path_tileset, path_tileset);

        map_tiles_alloc(p_level->p_map);
        level_save(p_level, level_load_path, o_editor.path_tileset, o_editor.path_music);

        // load the tileset image so the level is editable without having to reload from file
        image_load(o_editor.p_level->p_map->p_tileset->p_image, o_editor.path_tileset, renderer, NULL);
      }
    }

    if (!o_editor.p_level->p_map)
    {
      printf("Cannot load level at '%s'\n", level_load_path);
      return EXIT_FAILURE;
    }
  }
  else
  {
    exit(0);
  }

  //editor states
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

  SDL_Quit();
  return EXIT_SUCCESS;
}
