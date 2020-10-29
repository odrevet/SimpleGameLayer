#ifndef SHAPES_H
#define SHAPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#include <image.h>

#include "globals.h"
#include "level.h"

#define SHAPE_NB 7
#define SHAPE_SIZE 4

typedef enum e_shape_type
{
    SHAPE_BOX,
    SHAPE_BAR,
    SHAPE_T,
    SHAPE_L,
    SHAPE_RL,
    SHAPE_S,
    SHAPE_RS
} shape_type;

typedef struct t_shape
{
    char layout[SHAPE_SIZE][SHAPE_SIZE];
    shape_type type;
    short len;
    int x;
    int y;
} shape;

void shape_set(shape *p_shape, shape_type type);
void shape_default_coord(shape *p_shape);
void shape_draw(shape *p_shape, image *p_image_block, SDL_Renderer *renderer);
char check_shape_level(shape *p_shape);

#endif
