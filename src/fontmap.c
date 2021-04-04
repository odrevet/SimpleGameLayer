#include "fontmap.h"

size_t _find_layout(fontmap *p_fontmap, char character)
{
  for (unsigned int i = 0; i < strlen(p_fontmap->layout); i++)
  {
    if (character == p_fontmap->layout[i])
    {
      return i;
    }
  }
  return -1;
}

void fontmap_print(fontmap *p_fontmap,
                   int x, int y,
                   SDL_Renderer *renderer,
                   const char *text)
{
  const int FONTMAP_SIZE = p_fontmap->character_size;

  SDL_Rect rect_src;
  SDL_Rect rect_dest;
  rect_dest.x = x;
  rect_dest.y = y;
  rect_dest.h = FONTMAP_SIZE;
  rect_dest.w = FONTMAP_SIZE;

  rect_src.x = 0;
  rect_src.y = 0;
  rect_src.h = FONTMAP_SIZE;
  rect_src.w = FONTMAP_SIZE;

  for (int show = 0; text[show] != '\0'; show++)
  {
    int index = _find_layout(p_fontmap, text[show]);

    switch (text[show])
    {
    case '\n':
      rect_dest.y += FONTMAP_SIZE + 1;
      rect_dest.x = x;
      break;
    case '\t':
      rect_dest.x += FONTMAP_SIZE + 4;
      break;
    case ' ':
      rect_dest.x += FONTMAP_SIZE + 1;
      break;
    case '\\':
      if (show + 1 < strlen(text) && text[show + 1] == 'n')
      {
        rect_dest.y += FONTMAP_SIZE + 1;
        rect_dest.x = x;
        show++;
      }
      break;
    default:
      rect_src.x = index * FONTMAP_SIZE;
      image_draw_part(p_fontmap->p_image, renderer, rect_dest.x, rect_dest.y, &rect_src);
      rect_dest.x += FONTMAP_SIZE + 1;
    }
  }
}

int fontmap_printf(fontmap *p_fontmap,
                   int x, int y,
                   SDL_Renderer *renderer,
                   const char *format, ...)
{
  va_list list;
  va_start(list, format);
  char text[BUFSIZ];
  vsprintf(text, format, list);
  fontmap_print(p_fontmap, x, y, renderer, text);
  va_end(list);
  return strlen(text);
}
