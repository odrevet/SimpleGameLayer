#include "NPC.h"

void NPC_init(NPC *p_NPC, image *p_image_NPC, SDL_Renderer *renderer)
{
    tileset *p_tileset = malloc(sizeof(tileset));
    p_tileset->p_image = p_image_NPC;
    tileset_init(p_tileset, 3, 4, 0, 4);

    sprite *p_sprite = malloc(sizeof(sprite));
    p_sprite->vel_x = 0;
    p_sprite->vel_y = 0;
    p_sprite->p_tileset = p_tileset;
    p_NPC->p_sprite = p_sprite;
    p_NPC->p_on_action_param = NULL;

    animation_init(p_tileset->v_animation + DOWN, false, 4, 10, 0);
    tileset_set_frame(p_tileset, DOWN, 0, 00, 0, 14, 21);
    tileset_set_frame(p_tileset, DOWN, 1, 14, 0, 14, 21);
    tileset_set_frame(p_tileset, DOWN, 2, 00, 0, 14, 21);
    tileset_set_frame(p_tileset, DOWN, 3, 28, 0, 14, 21);

    animation_init(p_tileset->v_animation + UP, false, 4, 10, 0);
    tileset_set_frame(p_tileset, UP, 0, 00, 42, 14, 21);
    tileset_set_frame(p_tileset, UP, 1, 14, 42, 14, 21);
    tileset_set_frame(p_tileset, UP, 2, 00, 42, 14, 21);
    tileset_set_frame(p_tileset, UP, 3, 28, 42, 14, 21);

    animation_init(p_tileset->v_animation + LEFT, false, 4, 10, 0);
    tileset_set_frame(p_tileset, LEFT, 0, 00, 64, 14, 21);
    tileset_set_frame(p_tileset, LEFT, 1, 14, 64, 14, 21);
    tileset_set_frame(p_tileset, LEFT, 2, 00, 63, 14, 21);
    tileset_set_frame(p_tileset, LEFT, 3, 28, 64, 14, 21);

    animation_init(p_tileset->v_animation + RIGHT, false, 4, 10, 0);
    tileset_set_frame(p_tileset, RIGHT, 0, 00, 21, 14, 21);
    tileset_set_frame(p_tileset, RIGHT, 1, 14, 21, 14, 21);
    tileset_set_frame(p_tileset, RIGHT, 2, 00, 21, 14, 21);
    tileset_set_frame(p_tileset, RIGHT, 3, 28, 21, 14, 21);
}

void NPC_free(NPC *p_NPC)
{
    // TODO free all allocated memory except image of tilset which is shared 

    //sprite_free(p_NPC->p_sprite);
    //free(p_NPC->p_sprite);
}