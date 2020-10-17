#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

/**
    @brief struct t_timer
    The purpose of a timer struct is to get the elspsed time from an event.
    SDL_GetTicks give the elapsed time from the begin of the application, in the case
    we want to use it to see if the time in an event is over, we need to get the time elapsed
    not from the begin of the application, but from the begin of the event.
    So we just create a timer at the begining of the event we want to mesure and then timer_get_ticks
    will give us the right time with the formulea ticks = SDL_GetTicks () - timer->start_ticks;
*/
typedef struct t_timer
{
    int start_ticks;
    int paused_ticks;
    bool paused;
    bool started;
} timer;

void timer_init(timer *p_timer);
void timer_start(timer *p_timer);
void timer_stop(timer *p_timer);
void timer_pause(timer *p_timer);
void timer_unpause(timer *p_timer);
int timer_get_ticks(timer *p_timer);
bool timer_is_started(timer *p_timer);
bool timer_is_paused(timer *p_timer);

#endif
