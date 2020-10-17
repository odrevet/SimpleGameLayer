#ifndef PLACE_H
#define PLACE_H

#include <SDL.h>

#include <image.h>

#include "hotspot.h"
#include "callbacks.h"
#include "callbacks_user.h"

typedef struct t_place
{
  char id[32];
  int image_count;
  image *v_image;
  SDL_Point *p_images_loc;
  int hotspot_count;
  hotspot *p_hotspots;
} place;

/**
   Assign default values to a place
 */
void place_init(place *p_place);

/**
   Read a file and assign values to a given place
   file format is
   NumberOfImages            (one per line)
   ImageFilename X Y
   NumberOfHotSpots
   X Y W H TYPE (one character) PARAMS (variatic)

   TYPE LIST
   L (link) PARAMS: DESTINATION_ID (string of max 8 characters)
   F (function) PARAMS: FUNCTION_ID (integer)
   T (text) PARAMS: TEXT TO DISPLAY
 */
void place_init_from_file(place *p_place, char *filepath, SDL_Renderer *renderer);

/**
   Draw all the pictures
 */
void place_draw(place *p_place, SDL_Renderer *renderer);

void place_free(place *p_place);

#endif
