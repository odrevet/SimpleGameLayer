#include "tileset.h"

void tileset_set_frame(tileset *p_tileset,
                       int animation_id,
                       int frame_index,
                       int x, int y, int w, int h)
{
  animation_set_frame(p_tileset->v_animation[animation_id].v_frame + frame_index, x, y, w, h);
}

void tileset_init(tileset *p_tileset, int animation_nb)
{
  p_tileset->animation_nb = animation_nb;
  p_tileset->v_animation = calloc(animation_nb, sizeof(animation));
}

void tileset_draw(tileset *p_tileset, SDL_Renderer *renderer, int x, int y, int animation_index)
{
  int frame_current = p_tileset->v_animation[animation_index].frame_current;
  SDL_Rect src = p_tileset->v_animation[animation_index].v_frame[frame_current];
  image_draw_part(p_tileset->p_image, renderer, x, y, &src);
}

void tileset_update_frame(tileset *p_tileset, int animation_index)
{
  animation *p_animation = p_tileset->v_animation + animation_index;
  animation_update(p_animation);
}

void tileset_free(tileset *p_tileset)
{
  for (int index_animation = 0; index_animation < p_tileset->animation_nb; index_animation++)
  {
    animation_free(p_tileset->v_animation + index_animation);
  }

  free(p_tileset->v_animation);
  p_tileset->v_animation = NULL;
  image_free(p_tileset->p_image);
  p_tileset->p_image = NULL;
}
