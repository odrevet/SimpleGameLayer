#include "dialog_box.h"

void dialog_box_show(fontmap *p_fontmap, char *text, int screen_height, int screen_width, SDL_Renderer *renderer)
{
  dialog o_dialog;
  o_dialog.p_fontmap = p_fontmap;

  SDL_Color color_border = {.r = 0, .g = 0, .b = 0};
  SDL_Color color_background = {.r = 25, .g = 25, .b = 255, .a = 100};
  style o_style = {.x = 10,
                   .y = screen_height / 2,
                   .h = screen_height / 2,
                   .w = screen_width - 20,
                   .border = color_border,
                   .bgcolor = color_background,
                   .padding = 5};
  o_dialog.o_style = o_style;

  int index_text = 0;
  int text_size = strlen(text);
  o_dialog.text = calloc(text_size, sizeof(char));

  bool done = false;
  while (!done)
  {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event))
    {
      if (sdl_event.type == SDL_QUIT)
      {
        done = true;
      }
      if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.repeat == 0)
      {
        if (sdl_event.key.keysym.sym == SDLK_RETURN && index_text == text_size)
        {
          done = true;
        }
        else
        {
          strcpy(o_dialog.text, text);
          index_text = text_size;
        }
      }
    }

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_GetScancodeFromKey(SDLK_q)])
    {
      done = true;
      break;
    }

    if (index_text < text_size)
    {
      o_dialog.text[index_text] = text[index_text];
      index_text++;
    }

    dialog_draw(&o_dialog, renderer);
    //game_draw(p_game, renderer);
  }

  free(o_dialog.text);
}
