#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include <SDL.h>

#include <UI/dialog.h>

#include "game.h"

void dialog_box_show(game *p_game, char *text, SDL_Renderer *renderer);
#endif
