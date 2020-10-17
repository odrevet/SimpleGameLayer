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

//extern struct t_shape;

char level[LEVEL_HEIGHT][LEVEL_WIDTH];

void level_init(void);
void level_draw(SDL_Renderer *renderer,
                struct t_shape *p_shape,
                image *p_image_block);
char level_check_game_over(void);
void level_remove_line(int line_nb);
/**
   @bried check if line are completed
   @param index of completed lines
   @return the number of completed lines
 */
int level_check_line(int *rem_tab);
void level_add_shape(struct t_shape *p_shape);
int level_get_at(int x, int y);

#endif
