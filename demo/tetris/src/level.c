#include "level.h"

void level_init(void)
{
  for (int i = 0; i < LEVEL_HEIGHT; i++)
  {
    memset(level[i], 0, LEVEL_WIDTH);
  }
}

void level_draw(SDL_Renderer *renderer,
                struct t_shape *p_shape,
                image *p_image_block)
{
  char tmp[LEVEL_HEIGHT][LEVEL_WIDTH];
  memcpy(tmp, level, sizeof(char) * LEVEL_HEIGHT * LEVEL_WIDTH);

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

char level_check_game_over(void)
{
  int i;
  for (i = 0; i < LEVEL_WIDTH; i++)
  {
    if (level[0][i] >= 1)
      return 1;
  }
  return 0;
}

void level_remove_line(int line_nb)
{
  for (int i = line_nb; i > 0; i--)
  {
    memmove(level + i, level + i - 1, LEVEL_WIDTH);
  }
}

int level_check_line(int *rem_tab)
{
  int completed_line = 0;

  for (int j = 0; j < LEVEL_HEIGHT; j++)
  {
    int i = 0;
    while (i < LEVEL_WIDTH)
    {
      if (level[j][i] != 0)
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

void level_add_shape(struct t_shape *p_shape)
{
  for (int i = 0; i < p_shape->len; i++)
  {
    for (int j = 0; j < p_shape->len; j++)
    {
      if (p_shape->y + j >= 0 && p_shape->layout[j][i] != 0)
      {
        level[p_shape->y + j][p_shape->x + i] = p_shape->layout[j][i];
      }
    }
  }
}

int level_get_at(int x, int y)
{
  if (x < 0)
    return 0;
  else
    return level[x][y];
}
