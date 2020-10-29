#include "shapes.h"

void shape_set(shape *p_shape, shape_type type)
{
  char shape_bar[SHAPE_SIZE][SHAPE_SIZE] = {
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
  };

  char shape_box[SHAPE_SIZE][SHAPE_SIZE] = {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}};

  char shape_T[SHAPE_SIZE][SHAPE_SIZE] = {
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}};

  char shape_L[SHAPE_SIZE][SHAPE_SIZE] = {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}};

  char shape_RL[SHAPE_SIZE][SHAPE_SIZE] = {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}};

  char shape_S[SHAPE_SIZE][SHAPE_SIZE] = {
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}};

  char shape_RS[SHAPE_SIZE][SHAPE_SIZE] = {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}};

  char *tmp = NULL;

  switch (type)
  {
  case SHAPE_BOX:
    tmp = (char *)shape_box;
    p_shape->len = 2;
    break;
  case SHAPE_BAR:
    tmp = (char *)shape_bar;
    p_shape->len = 4;
    break;
  case SHAPE_T:
    tmp = (char *)shape_T;
    p_shape->len = 3;
    break;
  case SHAPE_L:
    tmp = (char *)shape_L;
    p_shape->len = 3;
    break;
  case SHAPE_RL:
    tmp = (char *)shape_RL;
    p_shape->len = 3;
    break;
  case SHAPE_S:
    tmp = (char *)shape_S;
    p_shape->len = 3;
    break;
  case SHAPE_RS:
    tmp = (char *)shape_RS;
    p_shape->len = 3;
    break;
  }

  memcpy(p_shape->layout, tmp, SHAPE_SIZE * SHAPE_SIZE * sizeof(char));
  p_shape->type = type;
}

void shape_default_coord(shape *p_shape)
{
  p_shape->x = LEVEL_WIDTH / 2;
  p_shape->y = -p_shape->len + 1;
}

void shape_draw(shape *p_shape, image *p_image_block, SDL_Renderer *renderer)
{
  for (int i = 0; i < p_shape->len; i++)
  {
    for (int j = 0; j < p_shape->len; j++)
    {
      if (p_shape->layout[j][i] == 1)
      {
        image_draw(p_image_block,
                   renderer,
                   p_shape->x * TILE_SIZE + i * TILE_SIZE,
                   p_shape->y * TILE_SIZE + j * TILE_SIZE);
      }
    }
  }
}

