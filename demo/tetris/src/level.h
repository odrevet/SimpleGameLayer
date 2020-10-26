#ifndef LEVEL_H
#define LEVEL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#include <image.h>

#include "globals.h"
#include "shapes.h"

#define LEVEL_HEIGHT 15
#define LEVEL_WIDTH 12

void level_init(char p_level[][LEVEL_WIDTH]);
void level_draw(char p_level[][LEVEL_WIDTH],
		SDL_Renderer *renderer,
                struct t_shape *p_shape,
                image *p_image_block);
char level_check_game_over(char p_level[][LEVEL_WIDTH]);
void level_remove_line(char p_level[][LEVEL_WIDTH], int line_nb);
int level_check_line(char p_level[][LEVEL_WIDTH], int *rem_tab);
void level_add_shape(char p_level[][LEVEL_WIDTH], struct t_shape *p_shape);
int level_get_at(char p_level[][LEVEL_WIDTH], int x, int y);

#endif
