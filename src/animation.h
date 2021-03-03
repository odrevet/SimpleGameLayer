#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

typedef struct t_animation
{
    int frame_delay;   // time in sec before switch to next frame
    int frame_trigger; // count the remaining time before the next frame
    int frame_current; // number of the current frame in the animation
    int frame_default; // the default frame, when the animation start
    int frame_nb;      // number of frames in the animation
    SDL_Rect *v_frame; // rect of each frames
} animation;

/*
    Allocate `frame_nb` frames which must be freed with `animation_free`
*/
void animation_init(animation *p_animation,
                    bool have_loop,
                    int frame_nb,
                    int frame_delay,
                    int frame_default);

void animation_update(animation *p_animation);

void animation_set_frame(SDL_Rect *frame, int x, int y, int h, int w);

SDL_Rect animation_get_current_frame(animation *p_animation);

void animation_free(animation *p_animation);

#endif