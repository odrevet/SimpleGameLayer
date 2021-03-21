#ifndef HERO_H
#define HERO_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include <init.h>
#include <image.h>
#include <sprite.h>

#include "utils.h"

typedef struct t_hero
{
    sprite o_sprite;
    int health;
    int money;
} hero;

void hero_init(hero *p_hero, SDL_Renderer *renderer);
void hero_free(hero *p_hero);

#endif