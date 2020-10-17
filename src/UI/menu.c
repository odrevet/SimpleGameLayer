#include "menu.h"

void menu_init(menu *p_menu)
{
  p_menu->index_choice = 0;
  p_menu->entries_count = 0;
  p_menu->p_entries = NULL;
  p_menu->on_draw = NULL;
}

void menu_add_entry(menu *p_menu, char *text, entry_on_action on_action, void *param)
{
  p_menu->p_entries = realloc(p_menu->p_entries, (p_menu->entries_count + 1) * sizeof(entry));
  entry_init(p_menu->p_entries + p_menu->entries_count, text, on_action, param);
  p_menu->entries_count++;
}

int menu_draw(menu *p_menu, SDL_Renderer *renderer, fontmap *p_fontmap, char *cursor, void *param)
{

  if (p_menu->on_draw)
    p_menu->on_draw(param, renderer);

  //background color
  SDL_SetRenderDrawColor(renderer,
                         p_menu->o_style.bgcolor.r,
                         p_menu->o_style.bgcolor.g,
                         p_menu->o_style.bgcolor.b,
                         p_menu->o_style.bgcolor.a);
  SDL_Rect rect = {.x = p_menu->o_style.x,
                   .y = p_menu->o_style.y - p_menu->o_style.margin,
                   .h = p_menu->entries_count * p_fontmap->character_size + (p_menu->o_style.margin * (p_menu->entries_count + 1)),
                   .w = 100};
  SDL_RenderFillRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer,
                         p_menu->o_style.border.r,
                         p_menu->o_style.border.g,
                         p_menu->o_style.border.b,
                         p_menu->o_style.border.a);
  SDL_RenderDrawRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  //entries text
  for (int i = 0; i < p_menu->entries_count; i++)
  {
    fontmap_print(p_fontmap,
                  p_fontmap->character_size + p_menu->o_style.x,
                  p_menu->o_style.y + i * p_fontmap->character_size + i * p_menu->o_style.margin,
                  renderer,
                  p_menu->p_entries[i].text);
  }

  //curor character at current choice
  fontmap_print(p_fontmap,
                p_menu->o_style.x,
                p_menu->o_style.y + p_menu->index_choice * p_fontmap->character_size + p_menu->index_choice * p_menu->o_style.margin,
                renderer,
                cursor);

  SDL_RenderPresent(renderer);
  SDL_RenderClear(renderer);

  return 0;
}

void entry_init(entry *p_entry, char *text, entry_on_action on_action, void *param)
{
  p_entry->text = calloc(strlen(text), sizeof(char));
  p_entry->param = param;
  strcpy(p_entry->text, text);
  p_entry->on_action = on_action;
}

void menu_index_dec(menu *p_menu)
{
  if (p_menu->index_choice > 0)
    p_menu->index_choice--;
}

void menu_index_inc(menu *p_menu)
{
  if (p_menu->index_choice < p_menu->entries_count - 1)
    p_menu->index_choice++;
}

void menu_free(menu *p_menu)
{
  for (int i = 0; i < p_menu->entries_count; i++)
  {
    free(p_menu->p_entries[i].text);
  }
}
