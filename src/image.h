#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

/**
    @struct t_image
*/
typedef struct t_image
{
    int width;
    int height;
    SDL_Texture *p_texture;
    bool flip;
} image;

/**
    @param Full path of the file to load
    @param the sdl2 renderer
    the returned image is allocated and should be free using image_free
*/
void image_load(image *p_image, const char *full_path, SDL_Renderer *renderer);

/**
    @brief Draw an image
    @param the image to draw
    @param the SDL2 renderer to draw to
    @param the x coord to draw to
    @param the y coord to draw to
*/
bool image_draw(image *p_image, SDL_Renderer *renderer, int x, int y);

bool image_draw_resize(image *p_image, SDL_Renderer *renderer, int x, int y, int w, int h);

/**
    @brief Draw a part of an image
    @param the image to draw
    @param the SDL2 renderer to draw to
    @param the x coord to draw to
    @param the y coord to draw to
    @param the source rectangle in the image
*/
bool image_draw_part(image *p_image, SDL_Renderer *renderer, int x, int y, SDL_Rect *src);

void image_set_tint(image *p_image, int r, int g, int b);

void image_free(image *p_image);

#endif
