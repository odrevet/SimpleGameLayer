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
    IN_EDITOR,
} editor_state;

typedef struct t_editor
{
    level o_level;
    char *path_level;

    //selected tile in the tileset
    int tileset_selected_index;
    int tileset_selected_animated_index;
    bool tileset_selected_is_animated;

    // selected tile in the map
    int layer;
    int map_tile_index_x;
    int map_tile_index_y;

    // scroll location in the tile selector
    int tile_select_scroll_index_x;
    int tile_select_scroll_index_y;

    fontmap *p_fontmap;
    char *path_tileset;
    char *path_music;
} editor;

void editor_init(editor *p_editor);
editor_state editor_edit_layout(editor *p_editor, SDL_Renderer *renderer);
editor_state editor_tile_selection(editor *p_editor, SDL_Renderer *renderer);
editor_state editor_animated_tile_selection(editor *p_editor, SDL_Renderer *renderer);
void editor_render_tileset_animations(tileset *p_tileset, SDL_Renderer *renderer);
void editor_tileset_render_grid(tileset *p_tileset, SDL_Renderer *renderer);

#endif