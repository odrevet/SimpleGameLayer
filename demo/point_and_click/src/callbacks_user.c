#include "callbacks_user.h"
#include "game.h"

void callback_user_click_desk(hotspot *p_hotspot, void *param, SDL_Renderer *renderer)
{
  game *p_game = (game *)param;
  if (p_game->has_key == false)
  {
    notification("I FOUND A KEY", p_game, renderer);
    p_game->has_key = true;
  }
  else
  {
    notification("I ALDERLY CHECKED THE DESK", p_game, renderer);
  }
}

void callback_user_click_bedroom_door(hotspot *p_hotspot, void *param, SDL_Renderer *renderer)
{
  game *p_game = (game *)param;
  if (p_game->has_key == false)
  {
    notification("THE DOOR IS LOCKED", p_game, renderer);
  }
  else
  {
    notification("USED THE KEY", p_game, renderer);
    p_hotspot->param = "bedroom";
    callback_link(p_hotspot, p_game, renderer);
  }
}

void assign_callbacks()
{
  callbacks_user[0] = callback_user_click_desk;
  callbacks_user[1] = callback_user_click_bedroom_door;
}
