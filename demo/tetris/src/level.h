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

typedef struct t_shape shape;

void level_init(char p_level[][LEVEL_WIDTH]);
void level_draw(char *p_level[LEVEL_WIDTH], SDL_Renderer *renderer, shape *p_shape, image *p_image_block);
char level_check_game_over(char p_level[][LEVEL_WIDTH]);
void level_remove_line(char p_level[][LEVEL_WIDTH], int line_nb);
int level_check_line(char p_level[][LEVEL_WIDTH], int *rem_tab);
void level_add_shape(char p_level[][LEVEL_WIDTH], shape *p_shape);
char shape_move(char *p_level[], shape *p_shape, int x, int y);
int level_get_at(char p_level[][LEVEL_WIDTH], int x, int y);
char shape_rotate(char *p_level[], shape *p_shape, char dir);
#endif
