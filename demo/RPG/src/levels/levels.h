#ifndef LEVELS_H
#define LEVELS_H

#include <SDL.h>
#include <SDL_mixer.h>

#include <sound.h>

#include "../NPC.h"
#include "../game.h"
#include "../hero.h"
#include "../level.h"
#include "../dialog_box.h"
#include "../event.h"
#include "../chest.h"

void level_overworld(game *p_game, SDL_Renderer *renderer);
void level_cave(game *p_game, SDL_Renderer *renderer);
void level_town(game *p_game, SDL_Renderer *renderer);

#endif
