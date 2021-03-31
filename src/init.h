/**
    @brief  Helpers uppon SDL2 core
*/

#ifndef INIT
#define INIT

#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

void init(int flags);

/**
   @brief create an SDL window
   @param name
   @param width
   @param height
   @param flags
 */
SDL_Window *create_window(char *name, int *width, int *height, int flags);
#endif
