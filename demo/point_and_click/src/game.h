#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include <fontmap.h>
#include <image.h>
#include <sound.h>
#include <UI/menu.h>

#include "place.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

typedef struct t_game
{
  fontmap *p_fontmap;
  place *p_place;

  // Variables for the game story
  bool has_key;
} game;

typedef enum e_game_status
{
  STAY = 0,
  QUIT,
  MAIN_MENU,
  IN_GAME,
} game_state;

game_state state_main_menu(SDL_Renderer *renderer, game *p_game);
game_state state_in_game(SDL_Renderer *renderer, game *p_game);
game_state state_in_game_menu(SDL_Renderer *renderer, game *p_game);
game_state state_game_over(SDL_Renderer *renderer, game *p_game);
int menu_run(menu *p_menu, SDL_Renderer *renderer, fontmap *p_fontmap, char *cursor, game *p_game);
void game_on_click(game *p_game, SDL_Point *coords, SDL_Renderer *renderer);
void notification(const char *text, game *p_game, SDL_Renderer *renderer);

#endif
