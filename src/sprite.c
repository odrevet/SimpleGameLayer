#include "sprite.h"

void sprite_init(sprite *p_sprite, int x, int y, int animation_current, int animation_nb)
{
  p_sprite->x = x;
  p_sprite->y = y;
  p_sprite->animation_current = animation_current;
  tileset_init(p_sprite->p_tileset, animation_nb);
}

void sprite_draw(sprite *p_sprite, SDL_Renderer *renderer)
{
  tileset_draw(p_sprite->p_tileset, renderer, p_sprite->x, p_sprite->y, p_sprite->animation_current);
}

void sprite_draw_with_offset(sprite *p_sprite, SDL_Renderer *renderer, int offset_x, int offset_y)
{
  tileset_draw(p_sprite->p_tileset, renderer, p_sprite->x - offset_x, p_sprite->y - offset_y, p_sprite->animation_current);
}

void sprite_update_frame(sprite *p_sprite)
{
  tileset_update_frame(p_sprite->p_tileset, p_sprite->animation_current);
}

bool sprite_intersect(const sprite *p_sprite_a, const sprite *p_sprite_b)
{
  SDL_Rect rec_a = p_sprite_a->bounding_box;
  SDL_Rect rec_b = p_sprite_b->bounding_box;

  return !(rec_a.y + rec_a.h <= rec_b.y || //a bottom line lesser than b top line ?
           rec_a.y >= rec_b.y + rec_b.h || //a top line upper than b bottom line ?
           rec_a.x + rec_a.w <= rec_b.x || //a right line lesser than b left line ?
           rec_a.x >= rec_b.x + rec_b.w);  //a left line upper than b right line ?
}

SDL_Rect sprite_get_bounding_box(const sprite *p_sprite)
{
  return p_sprite->bounding_box;
}

void sprite_free(sprite *p_sprite)
{
  tileset_free(p_sprite->p_tileset);
}