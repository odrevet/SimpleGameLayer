#include "hero.h"

void hero_init(hero *p_hero, SDL_Renderer *renderer)
{
    p_hero->o_sprite.x = 64;
    p_hero->o_sprite.y = 64;
    p_hero->o_sprite.vel_x = 0;
    p_hero->o_sprite.vel_y = 0;
    p_hero->o_sprite.animation_current = DOWN;
    p_hero->health = 3;
    p_hero->money = 0;
}

void hero_free(hero *p_hero)
{
    //sprite_free(&p_hero->o_sprite);
}