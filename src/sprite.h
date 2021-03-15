#ifndef sprite_H
#define sprite_H

#include <SDL.h>

#include "tileset.h"

typedef struct t_sprite
{
   float x, y;
   float vel_x, vel_y;
   int animation_current;
   SDL_Rect bounding_box;
   tileset *p_tileset;
} sprite;

void sprite_init(sprite *p_sprite, int x, int y, int animation_current, int animation_nb);

void sprite_draw(sprite *p_sprite, SDL_Renderer *renderer);

void sprite_draw_with_offset(sprite *p_sprite, SDL_Renderer *renderer, int offset_x, int offset_y);

/**
   @brief Update the frame of the animated sprite if it is time (frame_delay)
   @note Increment frame_current, or put it at 0 if more than frame_nb (see t_animation)
   Put frame_trigger to 0.
*/
void sprite_update_frame(sprite *p_sprite);

bool sprite_intersect(const sprite *p_sprite_a, const sprite *p_sprite_b);

SDL_Rect sprite_get_bounding_box(const sprite *p_sprite);

void sprite_free(sprite *p_sprite);

#endif
