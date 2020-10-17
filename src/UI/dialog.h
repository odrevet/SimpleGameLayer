#ifndef DIALOG_H
#define DIALOG_H

#include <stdbool.h>

#include <SDL.h>

#include "../fontmap.h"
#include "style.h"

typedef struct t_dialog
{
  char *text;
  style o_style;
  fontmap *p_fontmap;
} dialog;

void dialog_draw(dialog *p_dialog, SDL_Renderer *renderer);

#endif
