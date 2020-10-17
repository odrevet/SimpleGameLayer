#include "timer.h"

void timer_init(timer *p_timer)
{
  p_timer->start_ticks = 0;
  p_timer->paused_ticks = 0;
  p_timer->paused = SDL_FALSE;
  p_timer->started = SDL_FALSE;
}

void timer_start(timer *p_timer)
{
  p_timer->started = true;
  p_timer->paused = false;
  p_timer->start_ticks = SDL_GetTicks();
}

void timer_stop(timer *p_timer)
{
  p_timer->started = false;
  p_timer->paused = false;
}

void timer_pause(timer *p_timer)
{
  if (p_timer->started == SDL_TRUE && p_timer->paused == SDL_FALSE)
  {
    p_timer->paused = SDL_TRUE;
    p_timer->paused_ticks = SDL_GetTicks() - p_timer->start_ticks;
  }
}

void timer_unpause(timer *p_timer)
{
  if (p_timer->paused == SDL_TRUE)
  {
    p_timer->paused = SDL_FALSE;

    p_timer->start_ticks = SDL_GetTicks() - p_timer->paused_ticks;
    p_timer->paused_ticks = 0;
  }
}

Sint32 timer_get_ticks(timer *p_timer)
{
  int ticks = 0;
  if (p_timer->started == SDL_TRUE)
  {
    if (p_timer->paused == SDL_TRUE)
    {
      ticks = p_timer->paused_ticks;
    }
    else
    {
      ticks = SDL_GetTicks() - p_timer->start_ticks;
    }
  }
  return ticks;
}

bool timer_is_started(timer *p_timer)
{
  return p_timer->started;
}

bool timer_is_paused(timer *p_timer)
{
  return p_timer->paused;
}
