#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <image.h>
#include <fontmap.h>
#include <tilemap.h>
#include <animation.h>

#include "tile_property.h"
#include "NPC.h"

// forward declaration
typedef struct t_warp warp;
//typedef struct t_NPC NPC;

typedef struct t_level
{
    char *path_tileset;
    tilemap *p_map;

    char *path_tile_property;
    tile_property ***p_tile_properties;

    char *path_music;
    Mix_Music *p_music;

    int warp_cout;
    warp *p_warp;

    int NPC_cout;
    NPC *p_NPC;
} level;

void level_init(level *p_level);
bool level_load(level *p_level, const char *pathfile, SDL_Renderer *renderer);
tile_property *level_parse_tiles_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property);
void level_free(level *p_level);
#endif
