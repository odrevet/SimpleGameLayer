#include "image.h"

void image_load(image *p_image, const char *full_path, SDL_Renderer *renderer)
{
  p_image->p_texture = IMG_LoadTexture(renderer, full_path);
  SDL_QueryTexture(p_image->p_texture, NULL, NULL, &p_image->width, &p_image->height);
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
