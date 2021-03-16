#include "hero.h"

void hero_init(hero *p_hero, SDL_Renderer *renderer)
{
    tileset *p_tileset = malloc(sizeof(tileset));

    p_hero->o_sprite.x = 64;
    p_hero->o_sprite.y = 64;
    p_hero->o_sprite.vel_x = 0;
    p_hero->o_sprite.vel_y = 0;
    p_hero->o_sprite.animation_current = DOWN;
    p_hero->o_sprite.p_tileset = p_tileset;

    tileset_init_from_file(p_tileset, "data/hero.tileset", renderer);
}

void hero_free(hero *p_hero)
{
    sprite_free(&p_hero->o_sprite);
}