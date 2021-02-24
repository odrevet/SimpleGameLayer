#include "tilemap.h"

void map_init(tilemap *p_map)
{
  p_map->o_camera.x = p_map->o_camera.y = 0;
  p_map->p_tiles = NULL;
  p_map->nb_layer = 0;
  p_map->width = 0;
  p_map->height = 0;
  p_map->v_animation = NULL;
}

void map_draw(tilemap *p_map, SDL_Renderer *renderer)
{
  int SCREEN_WIDTH, SCREEN_HEIGHT;
  SDL_RenderGetLogicalSize(renderer, &SCREEN_WIDTH, &SCREEN_HEIGHT);

  for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
  {
    int x_from = -(p_map->o_camera.x % p_map->tile_width);
    int x_to = x_from + SCREEN_WIDTH + (x_from == 0 ? 0 : p_map->tile_width);
    int y_from = -(p_map->o_camera.y % p_map->tile_height);
    int y_to = y_from + SCREEN_HEIGHT + (y_from == 0 ? 0 : p_map->tile_height);
    int tile_y_index = p_map->o_camera.y / p_map->tile_height;

    for (int y = y_from; y < y_to; y += p_map->tile_height)
    {
      int tile_x_index = p_map->o_camera.x / p_map->tile_width;
      for (int x = x_from; x < x_to; x += p_map->tile_width)
      {
        tile *p_tile = p_map->p_tiles[index_layer][tile_y_index] + tile_x_index;
        map_draw_tile(p_map, p_tile, x, y, renderer);

        if (++tile_x_index >= p_map->width)
          break;
      }

      if (++tile_y_index >= p_map->height)
        break;
    }
  }
}

void map_draw_tile(tilemap *p_map,
                   tile *p_tile,
                   int x,
                   int y,
                   SDL_Renderer *renderer)
{
  SDL_Rect src;

  if (p_tile->p_animation)
  {
    int frame_current = p_tile->p_animation->frame_current;
    src = p_tile->p_animation->v_frame[frame_current];
  }
  else
  {
    src.x = p_tile->x * p_map->tile_width;
    src.y = p_tile->y * p_map->tile_height;
    src.w = p_map->tile_width;
    src.h = p_map->tile_height;
  }

  image_draw_part(p_map->p_image, renderer, x, y, &src);
}

void map_tiles_alloc(tilemap *p_map)
{
  p_map->p_tiles = calloc(p_map->nb_layer, sizeof(tile **));
  for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
  {
    p_map->p_tiles[index_layer] = calloc(p_map->height, sizeof(tile *));
    for (int index_height = 0; index_height < p_map->height; index_height++)
    {
      p_map->p_tiles[index_layer][index_height] = calloc(p_map->width, sizeof(tile));
    }
  }
}

void map_tiles_free(tilemap *p_map)
{
  for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
  {
    for (int index_height = 0; index_height < p_map->height; index_height++)
    {
      free(p_map->p_tiles[index_layer][index_height]);
    }
    free(p_map->p_tiles[index_layer]);
  }

  free(p_map->p_tiles);
}