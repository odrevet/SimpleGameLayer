#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <SDL.h>

typedef enum E_HOTSPOT_TYPE
{
  HS_LINK,
  HS_TEXT,
  HS_FUNCTION
} HOTSPOT_TYPE;

typedef struct t_hotspot hotspot; //forward declaration

typedef void (*hotspot_on_click)(hotspot *, void *, SDL_Renderer *);

typedef struct t_hotspot
{
  SDL_Rect location;
  HOTSPOT_TYPE type;
  hotspot_on_click on_click;
  void *param; //generic parameter (link: destination id, text: text to display, function: function index)
} hotspot;

#endif
