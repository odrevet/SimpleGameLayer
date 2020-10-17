#ifndef GAME_H
#define GAME_H

#include "shapes.h"

typedef enum e_game_status
{
  STAY = -1,
  QUIT,
  IN_GAME,
  OPTION,
  GAME_OVER,
} game_state;

#define STATE_NUM 4

typedef struct t_game
{
  int score;
  int state[STATE_NUM]; //count completed lines (single, double, triple, tetris)
  fontmap *p_fontmap;
} game;

#endif
