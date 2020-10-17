#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>

#include "style.h"
#include "../fontmap.h"

typedef int (*entry_on_action)(void *, void *, SDL_Renderer *);
typedef void (*menu_on_draw)(void *, SDL_Renderer *);

typedef struct t_entry
{
  char *text;
  void *param;
  entry_on_action on_action;
} entry;

typedef struct t_menu
{
  int entries_count;
  int index_choice;
  entry *p_entries;
  style o_style;
  menu_on_draw on_draw;
} menu;

void menu_init(menu *p_menu);
void menu_add_entry(menu *p_entry, char *text, entry_on_action on_action, void *param);
int menu_draw(menu *p_menu, SDL_Renderer *renderer, fontmap *p_fontmap, char *cursor, void *param);
void entry_init(entry *p_entry, char *text, entry_on_action on_action, void *param);
void menu_index_dec(menu *p_menu);
void menu_index_inc(menu *p_menu);
void menu_free(menu *p_menu);

#endif
