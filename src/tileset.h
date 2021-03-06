#ifndef tileset_H
#define tileset_H

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

#include "image.h"
#include "animation.h"

typedef struct t_tileset
{
   int tile_height;
   int tile_width;
   image o_image;
   int animation_nb;
   animation *v_animation;
} tileset;

void tileset_set_frame(tileset *p_tileset,
                       int animation_id,
                       int frame_index,
                       int x, int y, int h, int w);

tileset *tileset_load_from_image(image *p_tileset);

void tileset_init(tileset *p_tileset, int animation_nb);
/**
   @brief Draw an animated image
*/
void tileset_draw(tileset *p_tileset, SDL_Renderer *renderer, int x, int y, int animation_index);

/**
   @brief Update the frame of the animated image if it is time (frame_delay)
   @note Increment frame_current, or put it at 0 if more than frame_nb (see t_animation)
   Put frame_trigger to 0.
*/
void tileset_update_frame(tileset *p_tileset, int animation_index);

/**
   @brief Get the tileset height
   @note charset height / frame_nb_y
*/

void tileset_free(tileset *p_tileset);

#endif
