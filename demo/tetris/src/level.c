#include "level.h"

void level_init(char p_level[][LEVEL_WIDTH])
{
  for (int i = 0; i < LEVEL_HEIGHT; i++)
  {
    memset(p_level[i], 0, LEVEL_WIDTH);
  }
}

void level_draw(char *p_level[LEVEL_WIDTH], SDL_Renderer *renderer, shape *p_shape, image *p_image_block)
{
  char tmp[LEVEL_HEIGHT][LEVEL_WIDTH];
  memcpy(tmp, p_level, sizeof(char) * LEVEL_HEIGHT * LEVEL_WIDTH);

  if (p_shape)
  {
    for (int i = 0; i < p_shape->len; i++)
    {
      for (int j = 0; j < p_shape->len; j++)
      {
        if (p_shape->y + j >= 0 && p_shape->layout[j][i] == 1)
        {
          tmp[p_shape->y + j][p_shape->x + i] = 1;
        }
      }
    }
  }

  for (int i = 0; i < LEVEL_HEIGHT; i++)
  {
    for (int j = 0; j < LEVEL_WIDTH; j++)
    {
      if (tmp[i][j])
      {
        image_draw(p_image_block,
                   renderer,
                   j * TILE_SIZE,
                   i * TILE_SIZE);
      }
    }
  }
}

char level_check_game_over(char p_level[][LEVEL_WIDTH])
{
  int i;
  for (i = 0; i < LEVEL_WIDTH; i++)
  {
    if (p_level[0][i] >= 1)
      return 1;
  }
  return 0;
}

void level_remove_line(char p_level[][LEVEL_WIDTH], int line_nb)
{
  for (int i = line_nb; i > 0; i--)
  {
    memmove(p_level + i, p_level + i - 1, LEVEL_WIDTH);
  }
}

int level_check_line(char p_level[][LEVEL_WIDTH], int *rem_tab)
{
  int completed_line = 0;

  for (int j = 0; j < LEVEL_HEIGHT; j++)
  {
    int i = 0;
    while (i < LEVEL_WIDTH)
    {
      if (p_level[j][i] != 0)
      {
        i++;
      }
      else
      {
        break;
      }
    }

    if (i == LEVEL_WIDTH)
    {
      rem_tab[completed_line] = j;
      completed_line++;
    }
  }

  return completed_line;
}

void level_add_shape(char p_level[][LEVEL_WIDTH], shape *p_shape)
{
  for (int i = 0; i < p_shape->len; i++)
  {
    for (int j = 0; j < p_shape->len; j++)
    {
      if (p_shape->y + j >= 0 && p_shape->layout[j][i] != 0)
      {
        p_level[p_shape->y + j][p_shape->x + i] = p_shape->layout[j][i];
      }
    }
  }
}

char shape_move(char *p_level[], shape *p_shape, int x, int y)
{
  for (int i = 0; i < SHAPE_SIZE; i++)
  {
    for (int j = 0; j < SHAPE_SIZE; j++)
    {
      if (p_shape->layout[j][i] >= 1)
      {
        if (level_get_at(p_level, p_shape->y + j + y,
                         p_shape->x + i + x) >= 1 ||
            p_shape->x + i + x < 0 ||
            p_shape->x + i + x >= LEVEL_WIDTH ||
            p_shape->y + j + y >= LEVEL_HEIGHT)
        {
          return 0;
        }
      }
    }
  }

  return 1;
}

int level_get_at(char p_level[][LEVEL_WIDTH], int x, int y)
{
  if (x < 0)
    return 0;
  else
    return p_level[x][y];
}

char shape_rotate(char *p_level[], shape *p_shape, char dir)
{
  shape tmp_shape;
  memset(tmp_shape.layout, 0, SHAPE_SIZE * SHAPE_SIZE * sizeof(char));
  tmp_shape.x = p_shape->x;
  tmp_shape.y = p_shape->y;

  if (dir == 0)
  {
    for (int i = 0; i < p_shape->len; i++)
    {
      for (int j = 0; j < p_shape->len; j++)
      {
        if (p_shape->layout[j][i] != 0)
        {
          int x_new = i;
          int y_new = 1 - (j - (p_shape->len - SHAPE_SIZE / 2));
          tmp_shape.layout[x_new][y_new] = p_shape->layout[j][i];
        }
      }
    }
  }
  else
  {
    for (int i = 0; i < p_shape->len; i++)
    {
      for (int j = 0; j < p_shape->len; j++)
      {
        if (p_shape->layout[j][i] != 0)
        {
          int x_new = 1 - (i - (p_shape->len - SHAPE_SIZE / 2));
          int y_new = j;
          tmp_shape.layout[x_new][y_new] = p_shape->layout[j][i];
        }
      }
    }
  }

  if (shape_move(p_level, &tmp_shape, 0, 0))
  {
    memcpy(p_shape->layout,
           tmp_shape.layout,
           SHAPE_SIZE * SHAPE_SIZE * sizeof(char));
    return 1;
  }
  else
  {
    return 0;
  }
}
