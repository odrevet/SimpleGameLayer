#include "hero.h"

void hero_init(hero *p_hero, SDL_Renderer *renderer)
{
    tileset *p_tileset = malloc(sizeof(tileset));
    p_tileset->p_image = malloc(sizeof(image));
    image_load(p_tileset->p_image, "gfx/character.png", renderer, NULL);
    tileset_init(p_tileset, 17, 8, 0, 4);

    sprite *p_sprite = malloc(sizeof(sprite));
    p_sprite->x = 64;
    p_sprite->y = 61;
    p_sprite->vel_x = 0;
    p_sprite->vel_y = 0;
    p_sprite->p_tileset = p_tileset;

    p_hero->p_sprite = p_sprite;

    animation_init(p_tileset->v_animation + DOWN, false, 4, 10, 0);
    tileset_set_frame(p_tileset, DOWN, 0, 00, 0, 16, 32);
    tileset_set_frame(p_tileset, DOWN, 1, 16, 0, 16, 32);
    tileset_set_frame(p_tileset, DOWN, 2, 32, 0, 16, 32);
    tileset_set_frame(p_tileset, DOWN, 3, 48, 0, 16, 32);

    animation_init(p_tileset->v_animation + UP, false, 4, 10, 0);
    tileset_set_frame(p_tileset, UP, 0, 00, 64, 16, 32);
    tileset_set_frame(p_tileset, UP, 1, 16, 64, 16, 32);
    tileset_set_frame(p_tileset, UP, 2, 32, 64, 16, 32);
    tileset_set_frame(p_tileset, UP, 3, 48, 64, 16, 32);

    animation_init(p_tileset->v_animation + LEFT, false, 4, 10, 0);
    tileset_set_frame(p_tileset, LEFT, 0, 00, 96, 16, 32);
    tileset_set_frame(p_tileset, LEFT, 1, 16, 96, 16, 32);
    tileset_set_frame(p_tileset, LEFT, 2, 32, 96, 16, 32);
    tileset_set_frame(p_tileset, LEFT, 3, 48, 96, 16, 32);

    animation_init(p_tileset->v_animation + RIGHT, false, 4, 10, 0);
    tileset_set_frame(p_tileset, RIGHT, 0, 00, 32, 16, 32);
    tileset_set_frame(p_tileset, RIGHT, 1, 16, 32, 16, 32);
    tileset_set_frame(p_tileset, RIGHT, 2, 32, 32, 16, 32);
    tileset_set_frame(p_tileset, RIGHT, 3, 48, 32, 16, 32);
}