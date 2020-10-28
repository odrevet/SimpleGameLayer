#include "callbacks.h"
#include "game.h"

void callback_link(hotspot *p_hotspot, void *param, SDL_Renderer *renderer)
{
#ifdef DEBUG
  printf("CALLBACK LINK WITH PARAM %s\n", (char *)p_hotspot->param);
#endif
  game *p_game = (game *)param;
  char *id = (char *)p_hotspot->param;
  strcpy(p_game->p_place->id, id);
  place_free(p_game->p_place);
  char dest[256];
  sprintf(dest, "res/%s", id);
  place_init_from_file(p_game->p_place, p_game->on_click, dest, renderer);
}

void callback_text(hotspot *p_hotspot, void *param, SDL_Renderer *renderer)
{
#ifdef DEBUG
  printf("CALLBACK TEXT WITH PARAM %s\n", (char *)p_hotspot->param);
#endif
  game *p_game = (game *)param;
  char *text = (char *)p_hotspot->param;
  notification(text, p_game, renderer);
}
