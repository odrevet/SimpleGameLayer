#include "tileset.h"

void tileset_set_frame(tileset *p_tileset,
                       int animation_id,
                       int frame_index,
                       int x, int y, int w, int h)
{
  animation_set_frame(p_tileset->v_animation[animation_id].v_frame + frame_index, x, y, w, h);
}

void tileset_init(tileset *p_tileset,
                  int frame_nb_x,
                  int frame_nb_y,
                  int animation_current,
                  int animation_nb)
{
  p_tileset->frame_nb_x = frame_nb_x;
  p_tileset->frame_nb_y = frame_nb_y;
  p_tileset->animation_current = animation_current;
  p_tileset->animation_nb = animation_nb;
  p_tileset->v_animation = calloc(animation_nb, sizeof(animation));
}

void tileset_draw(tileset *p_tileset, SDL_Renderer *renderer, int x, int y)
{
  int frame_current = p_tileset->v_animation[p_tileset->animation_current].frame_current;
  SDL_Rect src = tileset_get_current_animation(p_tileset)->v_frame[frame_current];
  image_draw_part(p_tileset->p_image,
                  renderer,
                  x, y,
                  &src);
}

void tileset_update_frame(tileset *p_tileset)
{
  animation *p_animation = tileset_get_current_animation(p_tileset);
  if ((p_animation->frame_delay >= 0) && (--p_animation->frame_trigger <= 0))
  {
    if (p_animation->frame_current + 1 >= p_animation->frame_nb)
    {
      p_animation->frame_current = 0;
    }
    else
    {
      p_animation->frame_current++;
    }

    p_animation->frame_trigger = p_animation->frame_delay;

    if (!p_tileset->have_loop && p_animation->frame_current == 0)
    {
      p_tileset->is_paused = true;
    }
  }
}

int tileset_get_height(tileset *p_tileset)
{
  return p_tileset->p_image->height / p_tileset->frame_nb_y;
}

int tileset_get_width(tileset *p_tileset)
{
  return p_tileset->p_image->width / p_tileset->frame_nb_x;
}

animation *tileset_get_current_animation(tileset *p_tileset)
{
  return p_tileset->v_animation + p_tileset->animation_current;
}

void tileset_free(tileset *p_tileset)
{
  for (int i = 0; i < p_tileset->animation_nb; i++)
  {
    animation_free(p_tileset->v_animation + i);
  }

  free(p_tileset->v_animation);
  p_tileset->v_animation = NULL;
}
