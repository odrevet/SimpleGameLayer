#include "game.h"

void game_draw(game *p_game, SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE);
  place_draw(p_game->p_place, renderer);
}

void game_on_click(game *p_game, SDL_Point *coords, SDL_Renderer *renderer)
{
#ifdef DEBUG
  printf("CLICK AT %d %d\n", coords->x, coords->y);
#endif

  place *p_place = p_game->p_place;

  for (int i = 0; i < p_place->hotspot_count; i++)
  {
    hotspot *p_hotspot = p_place->p_hotspots + i;
    if (SDL_PointInRect(coords, &p_hotspot->location))
    {

#ifdef DEBUG
      printf("CLICK AT HOTSPOT #%d \n", i);
#endif

      if (p_hotspot->on_click)
      {
        p_hotspot->on_click(p_hotspot, p_game, renderer);
      }
    }
  }
}

game_state state_in_game(SDL_Renderer *renderer, game *p_game)
{
  bool done = false;
  game_state ret_code = QUIT;

  //create the start place
  place o_place;
  place_init(&o_place);
  strcpy(o_place.id, "start");
  p_game->p_place = &o_place;
  place_init_from_file(p_game->p_place, p_game->on_click, "res/start", renderer);

  while (!done)
  {
    // single input
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event))
    {
      if (sdl_event.type == SDL_QUIT)
      {
        done = true;
      }
      else if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.repeat == 0)
      {
        switch (sdl_event.key.keysym.sym)
        {
#ifdef DEBUG
        case SDLK_r:
          printf("RELOAD PLACE %s\n", p_game->p_place->id);
          place_free(p_game->p_place);
          char dest[256];
          sprintf(dest, "res/%s", p_game->p_place->id);
          place_init_from_file(p_game->p_place, p_game->on_click ,dest, renderer);
          break;
#endif
        }
      }
      else if (sdl_event.type == SDL_MOUSEBUTTONDOWN)
      {
        SDL_Point mouse_coord;
        mouse_coord.x = sdl_event.button.x;
        mouse_coord.y = sdl_event.button.y;
        game_on_click(p_game, &mouse_coord, renderer);
      }
    }

    //repeated input
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //quit
    if (state[SDL_GetScancodeFromKey(SDLK_q)])
    {
      done = true;
      ret_code = QUIT;
      break;
    }

    if (state[SDL_GetScancodeFromKey(SDLK_m)])
    {
      ret_code = state_in_game_menu(renderer, p_game);
      if (ret_code == QUIT)
        done = true;
    }

    //update display
    game_draw(p_game, renderer);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
  }

  return ret_code;
}

int menu_entry_stay(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  return STAY;
}

int menu_entry_new_game(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  return IN_GAME;
}

int menu_entry_quit(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  return QUIT;
}

void game_draw_dim(void *param, SDL_Renderer *renderer)
{
  game *p_game = (game *)param;
  game_draw(p_game, renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
  SDL_Rect rect = {.x = 0,
                   .y = 0,
                   .h = SCREEN_HEIGHT,
                   .w = SCREEN_WIDTH};
  SDL_RenderFillRect(renderer, &rect);
}

void menu_set_default_style(menu *p_menu)
{
  p_menu->o_style.padding = 10;
  p_menu->o_style.x = 20;
  p_menu->o_style.y = 20;
  SDL_Color border = {.r = 200, .g = 200, .b = 200, .a = 255};
  p_menu->o_style.border = border;
  SDL_Color bgcolor = {.r = 100, .g = 100, .b = 100, .a = 200};
  p_menu->o_style.bgcolor = bgcolor;
}

int menu_entry_callback_main_menu(void *param_menu, void *param_entry, SDL_Renderer *renderer)
{
  return MAIN_MENU;
}

game_state state_in_game_menu(SDL_Renderer *renderer, game *p_game)
{
  game_state ret_code = IN_GAME;
  menu o_menu;
  menu_init(&o_menu);
  menu_set_default_style(&o_menu);
  o_menu.on_draw = game_draw_dim;

  menu_add_entry(&o_menu, "RETURN", menu_entry_stay, NULL);
  menu_add_entry(&o_menu, "QUIT", menu_entry_quit, NULL);

  ret_code = menu_run(&o_menu, renderer, p_game->p_fontmap, "-", p_game);

  menu_free(&o_menu);

  return ret_code;
}

game_state state_main_menu(SDL_Renderer *renderer, game *p_game)
{
  game_state ret_code = IN_GAME;

  menu o_menu;
  menu_init(&o_menu);
  menu_set_default_style(&o_menu);

  menu_add_entry(&o_menu, "NEW", menu_entry_new_game, NULL);
  menu_add_entry(&o_menu, "QUIT", menu_entry_quit, NULL);

  ret_code = menu_run(&o_menu, renderer, p_game->p_fontmap, "-", p_game);

  menu_free(&o_menu);

  return ret_code;
}

int menu_run(menu *p_menu, SDL_Renderer *renderer, fontmap *p_fontmap, char *cursor, game *p_game)
{
  int ret_code;
  bool menu_done = false;

  while (!menu_done)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_RETURN:
          ret_code = (p_menu->p_entries + p_menu->index_choice)->on_action(p_game, p_menu->p_entries[p_menu->index_choice].param, renderer);
          menu_done = true;
          break;
        case SDLK_UP:
          menu_index_dec(p_menu);
          break;
        case SDLK_DOWN:
          menu_index_inc(p_menu);
          break;
        }
      }
    }
    menu_draw(p_menu, renderer, p_game->p_fontmap, cursor, p_game);
  }
  return ret_code;
}

void notification(const char *text, game *p_game, SDL_Renderer *renderer)
{
  dialog o_dialog;
  o_dialog.text = text;
  o_dialog.p_fontmap = p_game->p_fontmap;
  style o_style;
  o_style.padding = 5;
  o_style.x = 10;
  o_style.y = 10;
  SDL_Color color_border = {155, 150, 250};
  SDL_Color color_bg = {100, 150, 250};
  o_style.border = color_border;
  o_style.bgcolor = color_bg;
  o_dialog.o_style = o_style;
  bool done = false;
  while (!done)
  {
    place_draw(p_game->p_place, renderer);
    dialog_draw(&o_dialog, renderer);

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if (state[SDL_GetScancodeFromKey(SDLK_RETURN)])
    {
      done = true;
      break;
    }
  }
}
