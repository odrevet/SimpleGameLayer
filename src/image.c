#include "image.h"

void image_load(image *p_image, const char *full_path, SDL_Renderer *renderer, SDL_Color *p_transparency)
{
  SDL_Surface *surface = IMG_Load(full_path);

  if (!surface)
  {
    printf("Error while loading %s: %s\n", full_path, IMG_GetError());
  }
  else
  {
    p_image->width = surface->w;
    p_image->height = surface->h;
    
    //transparency
    if (p_transparency)
    {
      SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, p_transparency->r, p_transparency->g, p_transparency->b));
    }

    p_image->p_texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
  }
  p_image->flip = false;
}

bool image_draw(image *p_image, SDL_Renderer *renderer, int x, int y)
{
  SDL_Rect rect_dest = {.x = x, .y = y, .h = p_image->height, .w = p_image->width};
  int retcode = SDL_RenderCopy(renderer, p_image->p_texture, NULL, &rect_dest);
  return (retcode > 0);
}

bool image_draw_resize(image *p_image, SDL_Renderer *renderer, int x, int y, int w, int h)
{
  SDL_Rect rect_dest = {.x = x, .y = y, .h = h, .w = w};

  int retcode = SDL_RenderCopy(renderer, p_image->p_texture, NULL, &rect_dest);
  return (retcode > 0);
}

bool image_draw_part(image *p_image, SDL_Renderer *renderer, int x, int y, SDL_Rect *src)
{
  SDL_Rect dest = {.x = x, .y = y, .w = src->w, .h = src->h};

  int retcode;

  if (p_image->flip)
  {
    retcode = SDL_RenderCopyEx(renderer,
                               p_image->p_texture,
                               src,
                               &dest,
                               0,
                               NULL,
                               true);
  }
  else
  {
    retcode = SDL_RenderCopy(renderer, p_image->p_texture, src, &dest);
  }

  return (retcode > 0);
}

void image_set_tint(image *p_image, int r, int g, int b)
{
  SDL_SetTextureColorMod(p_image->p_texture, r, g, b);
}

void image_free(image *p_image)
{
  if (p_image->p_texture)
  {
    SDL_DestroyTexture(p_image->p_texture);
  }
}
