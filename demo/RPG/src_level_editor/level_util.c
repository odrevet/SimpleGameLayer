#include "level_util.h"

bool level_save(level *p_level, const char *pathfile, char *path_tileset, char *path_music)
{
    FILE *fp = fopen(pathfile, "w");
    if (!fp)
    {
        printf("Cannot save map at %s\n", pathfile);
        return false;
    }

    tilemap *p_map = &p_level->o_tilemap;

    fprintf(fp, "%s\n", path_tileset);
    fprintf(fp, "%s\n", path_music);
    fprintf(fp, "%s\n", p_level->path_tile_property);
    fprintf(fp, "%d:%d:%d\n", p_map->width, p_map->height, p_map->nb_layer);

    for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
    {
        for (int index_y = 0; index_y < p_map->height; index_y++)
        {
            for (int index_x = 0; index_x < p_map->width; index_x++)
            {
                tile o_tile = p_map->p_tiles[index_layer][index_y][index_x];
                if (o_tile.is_animated)
                {
                    fprintf(fp, "a%d ", o_tile.id);
                }
                else if (o_tile.id == -1)
                {
                    fprintf(fp, "%c ", '_');
                }
                else
                {
                    fprintf(fp, "%d ", o_tile.id);
                }
            }
            fprintf(fp, "\n");
        }
    }

    fclose(fp);

    return true;
}

void map_add_row(tilemap *p_map)
{
    p_map->height++;
    const int index_height = p_map->height - 1;
    for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
    {
        p_map->p_tiles[index_layer] = realloc(p_map->p_tiles[index_layer], p_map->height * sizeof(tile *));
        p_map->p_tiles[index_layer][index_height] = calloc(p_map->width, sizeof(tile));
        for (int index_width = 0; index_width < p_map->width; index_width++)
        {
            p_map->p_tiles[index_layer][index_height][index_width].id = -1;
            p_map->p_tiles[index_layer][index_height][index_width].is_animated = false;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.x = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.y = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.h = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.w = 0;
        }
    }
}

void map_add_col(tilemap *p_map)
{
    p_map->width++;
    const int index_width = p_map->width - 1;
    for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
    {
        for (int index_height = 0; index_height < p_map->height; index_height++)
        {
            p_map->p_tiles[index_layer][index_height] = realloc(p_map->p_tiles[index_layer][index_height], p_map->width * sizeof(tile));
            p_map->p_tiles[index_layer][index_height][index_width].id = -1;
            p_map->p_tiles[index_layer][index_height][index_width].is_animated = false;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.x = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.y = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.h = 0;
            p_map->p_tiles[index_layer][index_height][index_width].o_frame.w = 0;
        }
    }
}

void map_shift_down(tilemap *p_map, int from)
{
    for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
    {
        for (int index_height = p_map->height - 1; index_height > from; index_height--)
        {
            p_map->p_tiles[index_layer][index_height] = p_map->p_tiles[index_layer][index_height - 1];
        }
    }
}

void map_shift_right(tilemap *p_map, int from)
{
    for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
    {
        for (int index_height = p_map->height - 1; index_height > from; index_height--)
        {
        }
    }
}