#ifndef EDITOR
#define EDITOR

#include <fontmap.h>

#include "../src/level.h"
#include "level_util.h"

#define SCREEN_WIDTH 15 * 16
#define SCREEN_HEIGHT 10 * 16

typedef enum e_editor_state
{
    STAY = -1,
    QUIT,
    LAYOUT_EDITOR,
    TILE_SELECTION,
    ANIMATED_TILE_SELECTION
} editor_state;

/*
    the editor support .map and .tileset files
    when editing a .map, p_tileset will point to p_level->o_tileset
    when editing a .tileset, p_level point to NULL and we alloc some memory to p_tileset
    we always refer to the editor's p_tileset when we want to access tilemap's tileset so we can share editor_tileset_ functions for both .map and .tileset
*/
typedef struct t_editor
{
    tileset *p_tileset;
    level o_level;
    char *path_level;

    //selected tile in the tileset
    int tileset_selected_index;
    int tileset_selected_animated_index;
    bool tileset_selected_is_animated;

    // selected tile in the map
    int layer;
    //int map_tile_index_x;  // to be removed
    //int map_tile_index_y;  // to be removed
    SDL_Rect map_tile_index;
    bool multiple_select;

    // scroll location in the tile selector
    int tile_select_scroll_index_x;
    int tile_select_scroll_index_y;

    fontmap *p_fontmap;
    char *path_music;
} editor;

void editor_init(editor *p_editor);
editor_state editor_edit_layout(editor *p_editor, SDL_Renderer *renderer);
editor_state editor_tile_selection(editor *p_editor, SDL_Renderer *renderer);
editor_state editor_animated_tile_selection(editor *p_editor, SDL_Renderer *renderer);
void editor_render_tileset_animations(tileset *p_tileset, SDL_Renderer *renderer);
void editor_tileset_render_grid(tileset *p_tileset, SDL_Renderer *renderer);

#endif