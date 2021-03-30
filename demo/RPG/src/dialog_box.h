#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <SDL.h>

#include <UI/dialog.h>

typedef void (*draw_callback)(void *, SDL_Renderer *);

void dialog_box_show(fontmap *p_fontmap, char *text, int screen_height, int screen_width, draw_callback *p_draw_callback, void *param, SDL_Renderer *renderer);

#endif