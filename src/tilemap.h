#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "image.h"
#include "animation.h"

typedef struct t_tile
{
  int id;
  int x;
  int y;
  animation *p_animation;
} tile;

typedef struct t_camera
{
  int x;
  int y;
} camera;

typedef struct t_tilemap
{
  camera o_camera;
  image *p_image;
  int width;
  int height;
  int nb_layer;
  int tile_width;
  int tile_height;
  tile ***p_tiles;
} tilemap;

void map_init(tilemap *p_map);
void map_draw(tilemap *p_map, SDL_Renderer *renderer);
void map_draw_tile(tilemap *p_map, tile *p_tile, int x, int y, SDL_Renderer *renderer);
void map_tiles_alloc(tilemap *p_map);
void map_tiles_free(tilemap *p_map);

#endif
