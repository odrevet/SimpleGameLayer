#include "animation.h"

void animation_init(animation *p_animation,
                    bool have_loop,
                    int frame_nb,
                    int frame_delay,
                    int frame_default)
{
    p_animation->frame_nb = frame_nb;
    p_animation->frame_delay = frame_delay;
    p_animation->frame_default = frame_default;
    p_animation->frame_trigger = 0;
    p_animation->frame_current = 0;
    p_animation->v_frame = calloc(frame_nb, sizeof(SDL_Rect));
}

void animation_update(animation *p_animation)
{
    if ((p_animation->frame_delay >= 0) && (--p_animation->frame_trigger <= 0))
    {
        if (p_animation->frame_current + 1 >= p_animation->frame_nb)
        {
            p_animation->frame_current = 0;
        }
        else
        {
            p_animation->frame_current++;
        }

        p_animation->frame_trigger = p_animation->frame_delay;
    }
}

void animation_set_frame(SDL_Rect *frame, int x, int y, int w, int h)
{
    frame->x = x;
    frame->y = y;
    frame->h = h;
    frame->w = w;
}

SDL_Rect animation_get_current_frame(animation *p_animation)
{
    return p_animation->v_frame[p_animation->frame_current];
}

void animation_free(animation *p_animation)
{
    free(p_animation->v_frame);
}