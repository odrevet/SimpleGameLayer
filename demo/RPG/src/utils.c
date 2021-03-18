#include "utils.h"

bool tileset_init_from_file(tileset *p_tileset, char *pathfile, SDL_Renderer *renderer)
{
  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    printf("cannot found tileset '%s'\n", pathfile);
    return false;
  }

  char buffer[256];

  // tileset
  fscanf(fp, "%s", buffer);
  image_load(&p_tileset->o_image, buffer, renderer, NULL);

  // tile size
  fscanf(fp, "%d:%d", &p_tileset->tile_width, &p_tileset->tile_height);

  // nb tile animated
  int nb_tile_animated;
  fscanf(fp, "%d", &nb_tile_animated);

  p_tileset->animation_nb = nb_tile_animated;
  p_tileset->v_animation = calloc(nb_tile_animated, sizeof(animation));

  for (int index_tile = 0; index_tile < nb_tile_animated; index_tile++)
  {
    int nb_frame;
    fscanf(fp, "%d", &nb_frame);
    animation_init(p_tileset->v_animation + index_tile, true, nb_frame, 10, 0);
    for (int index_frame = 0; index_frame < nb_frame; index_frame++)
    {
      int tile_index;
      fscanf(fp, "%d", &tile_index);

      int x = tile_index % (p_tileset->o_image.width / p_tileset->tile_width);
      int y = tile_index / (p_tileset->o_image.width / p_tileset->tile_width);
      animation_set_frame(p_tileset->v_animation[index_tile].v_frame + index_frame,
                          x * p_tileset->tile_width, y * p_tileset->tile_height,
                          p_tileset->tile_width,
                          p_tileset->tile_height);
    }
  }

  fclose(fp);
  return true;
}