#ifndef EDITOR
#define EDITOR

#include <fontmap.h>

#include "../src/level.h"

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
    level *p_level;
    char *path_level;
    SDL_Point mouse_coord;
    int tileset_selected_index; //selected tile in the tileset

    // selected tile in the map
    int layer;
    int map_tile_index_x;
    int map_tile_index_y;

    fontmap *p_fontmap;
} editor;

editor_state editor_edit_layout(editor *p_editor, SDL_Renderer *renderer);
editor_state editor_tile_selection(editor *p_editor, SDL_Renderer *renderer);

#endif