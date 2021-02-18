#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <image.h>
#include <sound.h>
#include <fontmap.h>
#include <tilemap.h>
#include <animation.h>

#include "tile_property.h"
#include "NPC.h"

// forward declaration
typedef struct t_warp warp;
typedef struct t_event event;

typedef struct t_level
{
    tilemap *p_map;
    char *path_tile_property;
    tile_property ***p_tile_properties;
    Mix_Music *p_music;

    int NPC_count;
    NPC *p_NPC;

    int event_count;
    event *p_event;
} level;

void level_init(level *p_level);
bool level_load(level *p_level, const char *pathfile, char **current_path_tileset, char **current_path_music, SDL_Renderer *renderer);
tile_property *level_parse_tiles_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property);
void level_free(level *p_level);
#endif
