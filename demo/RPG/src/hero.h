#ifndef HERO_H
#define HERO_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>

#include <init.h>
#include <image.h>
#include <sprite.h>

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3

typedef struct t_hero
{
    sprite *p_sprite;
} hero;

void hero_init(hero *p_hero, SDL_Renderer *renderer);

#endif