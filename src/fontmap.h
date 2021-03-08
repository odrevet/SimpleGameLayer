#ifndef FONTMAP_H
#define FONTMAP_H

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "image.h"

/**
   @brief use an image to print text
 */
typedef struct t_fontmap
{
  int character_size; //size for a character in pixel
  image *p_image;
  char *layout; //layout of characters in the image map
} fontmap;

/**
   @brief search a given character in the fontmap layout this function is used internally
   @param the fontmap
   @param the character to search
   @return the index of the character in the layout or -1 if the character is not found
 */
size_t _find_layout(fontmap *p_fontmap, char character);

/**
   @brief print text
   @param the fontmap
   @param x coord to print at
   @param y coord to print at
   @param SDL2 renderer
   @param text to print
 */
void fontmap_print(fontmap *p_fontmap,
                   int x,
                   int y,
                   SDL_Renderer *renderer,
                   const char *text);

/**
   @brief print formated text to destination
   @param the fontmap
   @param x coord to print at
   @param y coord to print at
   @param SDL2 renderer
   @param formated text to print
 */
int fontmap_printf(fontmap *p_fontmap,
                   int x,
                   int y,
                   SDL_Renderer *renderer,
                   const char *format, ...);

#endif
