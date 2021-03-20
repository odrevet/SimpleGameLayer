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
#include "utils.h"

// forward declaration
typedef struct t_warp warp;
typedef struct t_event event;
typedef struct t_chest chest;

typedef struct t_level
{
    int tileset_count;
    tileset *p_tileset;
    char **p_tileset_path;

    tilemap o_tilemap;
    char *path_tile_property;
    tile_property ***p_tile_properties;
    Mix_Music *p_music;

    int NPC_count;
    NPC *p_NPC;

    int event_count;
    event *p_event;

    int chest_count;
    chest *p_chest;

} level;

void level_init(level *p_level);
bool level_load(level *p_level, const char *pathfile, char **current_path_music, SDL_Renderer *renderer);
tile_property *level_parse_tiles_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property);

//Do not free tileset as it may be needed again between loading another level with the same tileset
void level_free(level *p_level);

#endif
