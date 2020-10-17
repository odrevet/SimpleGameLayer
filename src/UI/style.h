#ifndef STYLE_H
#define STYLE_H

#include <SDL.h>

typedef struct t_style
{
  int x;
  int y;
  int h;
  int w;
  SDL_Color border;
  SDL_Color bgcolor;
  int padding;
} style;

#endif
