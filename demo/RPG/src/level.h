#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer.h>

#include <image.h>
#include <sound.h>
#include <fontmap.h>
#include <tilemap.h>
#include <tileset.h>
#include <animation.h>

#include "event.h"
#include "utils.h"
#include "NPC.h"
#include "chest.h"
#include "tile_property.h"

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

    event_callback callbacks[4];
} level;

void level_init(level *p_level);
bool level_check_NPC_collid(level *p_level, SDL_Rect *p_bounding_box);
bool level_check_chest_collid(level *p_level, SDL_Rect *p_bounding_box);
bool level_map_init_from_file(level *p_level, const char *pathfile, SDL_Renderer *renderer);
tile_property *level_tile_props_init_from_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property);
bool level_init_from_file(level *p_level, char *pathfile, char **current_path_music, SDL_Renderer *renderer);
//Do not free tileset as it may be needed again between loading another level with the same tileset
void level_free_partial(level *p_level);

#endif
