#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include <image.h>
#include <tilemap.h>
#include <animation.h>

#define DOWN 0
#define UP 2
#define LEFT 3
#define RIGHT 1

bool tileset_init_from_file(tileset *p_tileset, char *pathfile, SDL_Renderer *renderer);

#endif 