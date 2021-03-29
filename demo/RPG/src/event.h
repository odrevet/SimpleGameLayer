#ifndef EVENT_H
#define EVENT_H

#include <SDL.h>

//#include "game.h"
#include "dialog_box.h"

typedef enum e_event_trigger
{
    ON_TILE_ENTER = 0,
    ON_BUTTON_PRESS
} event_trigger;

typedef enum e_event_type
{
    EVENT_TYPE_FUNCTION = 0,
    EVENT_TYPE_TEXT,
    EVENT_TYPE_WARP,
    EVENT_TYPE_MONEY
} event_type;

typedef void (*event_callback)(void *, SDL_Renderer *);

typedef struct t_event_param_warp
{
    int index_x;
    int index_y;
    event_callback p_level_addr;
} event_param_warp;

typedef struct t_event
{
    event_trigger o_event_trigger;
    event_type o_event_type;
    bool has_triggered; // check if auto event has alderly been triggered
    int index_src_x;    //position in the map
    int index_src_y;
    void *p_param;
} event;

void event_init(event *p_event, event_trigger event_trigger, event_type event_type, void *p_param, int index_x, int index_y);
//void event_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
//void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
//void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
//void event_money_exec(event *p_event, game *p_game, SDL_Renderer *renderer);
void event_free(event *p_event);

#endif