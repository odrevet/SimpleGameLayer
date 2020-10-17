#include "game.h"

int compar_sprite_depth(const void *a, const void *b)
{
  sprite *sprite_a = *(sprite **)a;
  sprite *sprite_b = *(sprite **)b;

  if (sprite_a->y < sprite_b->y)
    return -1;
  else if (sprite_a->y > sprite_b->y)
    return 1;
  else
    return 0;
}

void game_sort_sprites(game *p_game, int nb_sprites_to_draw, sprite **v_sprites_to_draw)
{
  for (int NPC_index = 0; NPC_index < p_game->p_level->NPC_cout; NPC_index++)
  {
    v_sprites_to_draw[NPC_index] = p_game->p_level->p_NPC[NPC_index].p_sprite;
  }
  v_sprites_to_draw[nb_sprites_to_draw - 1] = p_game->p_hero->p_sprite;
  qsort(v_sprites_to_draw, nb_sprites_to_draw, sizeof(sprite *), compar_sprite_depth);
}

void game_draw(game *p_game, SDL_Renderer *renderer)
{
  // sort sprite by y for the drawing order
  const int nb_sprites_to_draw = p_game->p_level->NPC_cout + 1;
  sprite *v_sprites_to_draw[nb_sprites_to_draw];
  game_sort_sprites(p_game, nb_sprites_to_draw, &v_sprites_to_draw);

  // draw map
  tilemap *p_map = p_game->p_level->p_map;
  map_draw(p_map, renderer);

  //draw sprites
  for (int sprite_index = 0; sprite_index < nb_sprites_to_draw; sprite_index++)
  {
    sprite_draw_with_offset(v_sprites_to_draw[sprite_index], renderer, p_map->o_camera.x, p_map->o_camera.y);
  }
}

bool game_check_NPC_collid(game *p_game)
{
  SDL_Rect hero_bounding_box = p_game->p_hero->p_sprite->bounding_box;
  hero_bounding_box.x += p_game->p_hero->p_sprite->vel_x;
  hero_bounding_box.y += p_game->p_hero->p_sprite->vel_y;

  for (int index_NPC = 0; index_NPC < p_game->p_level->NPC_cout; index_NPC++)
  {
    NPC *p_NPC = p_game->p_level->p_NPC + index_NPC;
    if (SDL_HasIntersection(&p_NPC->p_sprite->bounding_box, &hero_bounding_box))
    {
      return true;
    }
  }
  return false;
}

bool game_check_tile_walkable(tile_property ***p_tile_properties, tilemap *p_map, int x, int y)
{
  int index_x = x / p_map->tile_width;
  int index_y = y / p_map->tile_height;
  if (x < 0 || y < 0 || index_x < 0 || index_y < 0 || index_x >= p_map->width || index_y >= p_map->height)
  {
    return false;
  }

  return p_tile_properties[0][index_y][index_x].walkable;
}

void game_center_camera_on_hero(game *p_game)
{
  hero *p_hero = p_game->p_hero;
  tilemap *p_map = p_game->p_level->p_map;

  if (p_hero->p_sprite->y < SCREEN_HEIGHT / 2)
  {
    p_map->o_camera.y = 0;
  }
  else
  {
    p_map->o_camera.y = p_hero->p_sprite->y - (SCREEN_HEIGHT / 2);
    if (p_map->o_camera.y > p_map->height * p_map->tile_height - SCREEN_HEIGHT)
      p_map->o_camera.y = p_map->height * p_map->tile_height - SCREEN_HEIGHT;
  }

  if (p_hero->p_sprite->x < SCREEN_WIDTH / 2)
  {
    p_map->o_camera.x = 0;
  }
  else
  {
    p_map->o_camera.x = p_hero->p_sprite->x - (SCREEN_WIDTH / 2);
    if (p_map->o_camera.x > p_map->width * p_map->tile_width - SCREEN_WIDTH)
      p_map->o_camera.x = p_map->width * p_map->tile_width - SCREEN_WIDTH;
  }
}

void game_update(game *p_game)
{
  sprite *p_hero_sprite = p_game->p_hero->p_sprite;

  if (p_hero_sprite->vel_x || p_hero_sprite->vel_y)
  {
    // update camera position
    game_center_camera_on_hero(p_game);

    // update hero frame
    sprite_update_frame(p_hero_sprite);

    // check if next tiles are walkable
    int next_x = p_hero_sprite->bounding_box.x + p_hero_sprite->vel_x;
    int next_y = p_hero_sprite->bounding_box.y + p_hero_sprite->vel_y;
    tilemap *p_map = p_game->p_level->p_map;
    tile_property ***p_tile_properties = p_game->p_level->p_tile_properties;
    bool walkable = game_check_tile_walkable(p_tile_properties, p_map, next_x, next_y) &&
                    game_check_tile_walkable(p_tile_properties, p_map, next_x + p_hero_sprite->bounding_box.w, next_y) &&
                    game_check_tile_walkable(p_tile_properties, p_map, next_x + p_hero_sprite->bounding_box.w, next_y + p_hero_sprite->bounding_box.h) &&
                    game_check_tile_walkable(p_tile_properties, p_map, next_x, next_y + p_hero_sprite->bounding_box.h);

    // update hero position if map walkable and no collision with NPCs
    if (walkable && !game_check_NPC_collid(p_game))
    {
      p_hero_sprite->x += p_hero_sprite->vel_x;
      p_hero_sprite->y += p_hero_sprite->vel_y;
    }
  }

  //hero bounding box
  const int hero_bouding_box_margin_x = 5;
  const int hero_bouding_box_margin_y = 18;
  p_game->p_hero->p_sprite->bounding_box.x = p_game->p_hero->p_sprite->x + hero_bouding_box_margin_x;
  p_game->p_hero->p_sprite->bounding_box.y = p_game->p_hero->p_sprite->y + hero_bouding_box_margin_y;
  p_game->p_hero->p_sprite->bounding_box.w = sprite_get_width(p_game->p_hero->p_sprite) - 10;
  p_game->p_hero->p_sprite->bounding_box.h = sprite_get_height(p_game->p_hero->p_sprite) - 24;

  // NPCs bouding box
  const int NPC_bouding_box_margin_x = 0;
  const int NPC_bouding_box_margin_y = 12;
  for (int NPC_index = 0; NPC_index < p_game->p_level->NPC_cout;
       NPC_index++)
  {
    p_game->p_level->p_NPC[NPC_index].p_sprite->bounding_box.x = p_game->p_level->p_NPC[NPC_index].p_sprite->x + NPC_bouding_box_margin_x;
    p_game->p_level->p_NPC[NPC_index].p_sprite->bounding_box.y = p_game->p_level->p_NPC[NPC_index].p_sprite->y + NPC_bouding_box_margin_y;
    p_game->p_level->p_NPC[NPC_index].p_sprite->bounding_box.w = sprite_get_width(p_game->p_level->p_NPC[NPC_index].p_sprite) - NPC_bouding_box_margin_x;
    p_game->p_level->p_NPC[NPC_index].p_sprite->bounding_box.h = sprite_get_height(p_game->p_level->p_NPC[NPC_index].p_sprite) - NPC_bouding_box_margin_y;
  }
}