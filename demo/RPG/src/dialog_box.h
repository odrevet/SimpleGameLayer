#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <SDL.h>

#include <UI/dialog.h>

void dialog_box_show(fontmap *p_fontmap, char *text, int screen_height, int screen_width, SDL_Renderer *renderer);

#endif