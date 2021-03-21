#include "game.h"

int compar_sprite_depth(const void *a, const void *b)
{
  sprite *sprite_a = *(sprite **)a;
  sprite *sprite_b = *(sprite **)b;

  int sprite_a_bottom_y = sprite_a->bounding_box.y + sprite_a->bounding_box.h;
  int sprite_b_bottom_y = sprite_b->bounding_box.y + sprite_b->bounding_box.h;

  if (sprite_a_bottom_y < sprite_b_bottom_y)
    return -1;
  else if (sprite_a_bottom_y > sprite_b_bottom_y)
    return 1;
  else
    return 0;
}

void game_sort_sprites(game *p_game, int nb_sprites_to_draw, sprite *v_sprites_to_draw[])
{
  for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
  {
    v_sprites_to_draw[NPC_index] = &p_game->o_level.p_NPC[NPC_index].o_sprite;
  }
  v_sprites_to_draw[nb_sprites_to_draw - 1] = &p_game->o_hero.o_sprite;
  qsort(v_sprites_to_draw, nb_sprites_to_draw, sizeof(sprite *), compar_sprite_depth);
}

void game_draw(game *p_game, SDL_Renderer *renderer)
{
  // sort sprite by y for the drawing order
  const int nb_sprites_to_draw = p_game->o_level.NPC_count + 1;
  sprite *v_sprites_to_draw[nb_sprites_to_draw];
  game_sort_sprites(p_game, nb_sprites_to_draw, v_sprites_to_draw);

  // draw map
  tilemap *p_map = &p_game->o_level.o_tilemap;
  map_draw(p_map, renderer);

  //draw chests
  for (int chest_index = 0; chest_index < p_game->o_level.chest_count; chest_index++)
  {
    chest *p_chest = p_game->o_level.p_chest + chest_index;
    image_draw_part(&p_chest->o_sprite.p_tileset->o_image,
                    renderer,
                    p_chest->o_sprite.x - p_map->o_camera.x,
                    p_chest->o_sprite.y - p_map->o_camera.y,
                    p_chest->o_sprite.p_tileset->v_animation[0].v_frame + (p_chest->is_open ? 1 : 0));
  }

  //draw sprites
  for (int sprite_index = 0; sprite_index < nb_sprites_to_draw; sprite_index++)
  {
    sprite_draw_with_offset(v_sprites_to_draw[sprite_index], renderer, p_map->o_camera.x, p_map->o_camera.y);
  }

  //draw HUD
  for (int hero_health_index = 0; hero_health_index < p_game->o_hero.health; hero_health_index++)
  {
    int x = hero_health_index * 16;
    int y = 0;
    SDL_Rect src = {.x = 4 * 16, .y = 0, .w = 16, .h = 16};
    image_draw_part(&p_game->p_tilesets[1].o_image, renderer, x, y, &src);
  }
}

bool game_set_chest_is_open(game *p_game, chest *p_chest)
{
  for (int chest_id_index = 0; chest_id_index < p_game->opened_chest_id_count; chest_id_index++)
  {
    if (p_game->p_opened_chest_id[chest_id_index] == p_chest->id)
    {
      return true;
    }
  }
  return false;
}

bool game_check_NPC_collid(game *p_game)
{
  SDL_Rect hero_bounding_box = p_game->o_hero.o_sprite.bounding_box;
  hero_bounding_box.x += p_game->o_hero.o_sprite.vel_x;
  hero_bounding_box.y += p_game->o_hero.o_sprite.vel_y;

  for (int index_NPC = 0; index_NPC < p_game->o_level.NPC_count; index_NPC++)
  {
    NPC *p_NPC = p_game->o_level.p_NPC + index_NPC;
    if (SDL_HasIntersection(&p_NPC->o_sprite.bounding_box, &hero_bounding_box))
    {
      return true;
    }
  }
  return false;
}

bool game_check_chest_collid(game *p_game)
{
  SDL_Rect hero_bounding_box = p_game->o_hero.o_sprite.bounding_box;
  hero_bounding_box.x += p_game->o_hero.o_sprite.vel_x;
  hero_bounding_box.y += p_game->o_hero.o_sprite.vel_y;

  for (int index_chest = 0; index_chest < p_game->o_level.chest_count; index_chest++)
  {
    chest *p_chest = p_game->o_level.p_chest + index_chest;
    if (SDL_HasIntersection(&p_chest->o_sprite.bounding_box, &hero_bounding_box))
    {
      return true;
    }
  }
  return false;
}

bool game_check_tile_walkable(tile_property ***p_tile_properties, tilemap *p_map, int x, int y, int z)
{
  int index_x = x / p_map->p_tileset->tile_width;
  int index_y = y / p_map->p_tileset->tile_height;

  // check boundaries
  if (x < 0 || y < 0 || index_x < 0 || index_y < 0 || index_x >= p_map->width || index_y >= p_map->height)
  {
    return false;
  }

  return p_tile_properties[z][index_y][index_x].walkable;
}

void game_center_camera_on_hero(game *p_game)
{
  tilemap *p_map = &p_game->o_level.o_tilemap;

  if (p_game->o_hero.o_sprite.y < SCREEN_HEIGHT / 2)
  {
    p_map->o_camera.y = 0;
  }
  else
  {
    p_map->o_camera.y = p_game->o_hero.o_sprite.y - (SCREEN_HEIGHT / 2);
    if (p_map->o_camera.y > p_map->height * p_map->p_tileset->tile_height - SCREEN_HEIGHT)
      p_map->o_camera.y = p_map->height * p_map->p_tileset->tile_height - SCREEN_HEIGHT;
  }

  if (p_game->o_hero.o_sprite.x < SCREEN_WIDTH / 2)
  {
    p_map->o_camera.x = 0;
  }
  else
  {
    p_map->o_camera.x = p_game->o_hero.o_sprite.x - (SCREEN_WIDTH / 2);
    if (p_map->o_camera.x > p_map->width * p_map->p_tileset->tile_width - SCREEN_WIDTH)
      p_map->o_camera.x = p_map->width * p_map->p_tileset->tile_width - SCREEN_WIDTH;
  }
}

void game_update(game *p_game)
{
  // update map animations
  for (int animation_index = 0; animation_index < p_game->o_level.o_tilemap.p_tileset->animation_nb; animation_index++)
  {
    animation_update(p_game->o_level.o_tilemap.p_tileset->v_animation + animation_index);
  }

  // hero
  sprite *p_hero_sprite = &p_game->o_hero.o_sprite;

  if (p_hero_sprite->vel_x || p_hero_sprite->vel_y)
  {
    // update camera position
    game_center_camera_on_hero(p_game);

    // update hero frame
    sprite_update_frame(p_hero_sprite);

    // check if next tiles are walkable
    bool walkable = true;
    int next_x = p_hero_sprite->bounding_box.x + p_hero_sprite->vel_x;
    int next_y = p_hero_sprite->bounding_box.y + p_hero_sprite->vel_y;
    tilemap *p_map = &p_game->o_level.o_tilemap;
    tile_property ***p_tile_properties = p_game->o_level.p_tile_properties;

    int z = 0;
    do
    {
      walkable = game_check_tile_walkable(p_tile_properties, p_map, next_x, next_y, z) &&
                 game_check_tile_walkable(p_tile_properties, p_map, next_x + p_hero_sprite->bounding_box.w, next_y, z) &&
                 game_check_tile_walkable(p_tile_properties, p_map, next_x + p_hero_sprite->bounding_box.w, next_y + p_hero_sprite->bounding_box.h, z) &&
                 game_check_tile_walkable(p_tile_properties, p_map, next_x, next_y + p_hero_sprite->bounding_box.h, z);
      z++;
    } while (z < p_map->nb_layer && walkable == true);

    // update hero position if map walkable and no collisions
    if (walkable && !game_check_NPC_collid(p_game) && !game_check_chest_collid(p_game))
    {
      p_hero_sprite->x += p_hero_sprite->vel_x;
      p_hero_sprite->y += p_hero_sprite->vel_y;
    }
  }

  //hero bounding box
  const int hero_bouding_box_margin_x = 5;
  const int hero_bouding_box_margin_y = 18;
  p_game->o_hero.o_sprite.bounding_box.x = p_game->o_hero.o_sprite.x + hero_bouding_box_margin_x;
  p_game->o_hero.o_sprite.bounding_box.y = p_game->o_hero.o_sprite.y + hero_bouding_box_margin_y;
  p_game->o_hero.o_sprite.bounding_box.w = p_game->o_hero.o_sprite.p_tileset->tile_width - 10;
  p_game->o_hero.o_sprite.bounding_box.h = p_game->o_hero.o_sprite.p_tileset->tile_height - 24;

  // NPCs bouding box
  const int NPC_bouding_box_margin_x = 0;
  const int NPC_bouding_box_margin_y = 12;
  for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
  {
    p_game->o_level.p_NPC[NPC_index].o_sprite.bounding_box.x = p_game->o_level.p_NPC[NPC_index].o_sprite.x + NPC_bouding_box_margin_x;
    p_game->o_level.p_NPC[NPC_index].o_sprite.bounding_box.y = p_game->o_level.p_NPC[NPC_index].o_sprite.y + NPC_bouding_box_margin_y;
    p_game->o_level.p_NPC[NPC_index].o_sprite.bounding_box.w = p_game->o_level.p_NPC[NPC_index].o_sprite.p_tileset->tile_width - NPC_bouding_box_margin_x;
    p_game->o_level.p_NPC[NPC_index].o_sprite.bounding_box.h = p_game->o_level.p_NPC[NPC_index].o_sprite.p_tileset->tile_height - NPC_bouding_box_margin_y;
  }
}

void game_free(game *p_game)
{
  level_free(&p_game->o_level);
  hero_free(&p_game->o_hero);
  image_free(p_game->p_fontmap->p_image);
  free(p_game->path_music);

  for(int tileset_index = 0; tileset_index < p_game->tileset_count; tileset_index++)
  {
    tileset_free(p_game->p_tilesets + tileset_index);
    free(p_game->path_tilesets[tileset_index]);
  }
  free(p_game->p_tilesets);
  free(p_game->path_tilesets);
}