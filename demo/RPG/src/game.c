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

  //// draw HUD
  // health
  for (int hero_health_index = 0; hero_health_index < p_game->o_hero.health; hero_health_index++)
  {
    int x = hero_health_index * 16;
    int y = 0;
    SDL_Rect src = {.x = 4 * 16, .y = 0, .w = 16, .h = 16};
    image_draw_part(&p_game->p_tilesets[1].o_image, renderer, x, y, &src);
  }

  // money
  SDL_Rect src = {.x = 0, .y = 4 * 16, .w = 16, .h = 16};
  image_draw_part(&p_game->p_tilesets[1].o_image, renderer, 0, 20, &src);
  fontmap_printf(p_game->p_fontmap, 16, 24, renderer, "%d", p_game->o_hero.money);
}

bool game_get_chest_is_open(game *p_game, int chest_id)
{
  for (int chest_id_index = 0; chest_id_index < p_game->opened_chest_id_count; chest_id_index++)
  {
    if (p_game->p_opened_chest_id[chest_id_index] == chest_id)
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

    // update hero bounding box
    p_game->o_hero.o_sprite.bounding_box.x += p_game->o_hero.o_sprite.vel_x;
    p_game->o_hero.o_sprite.bounding_box.y += p_game->o_hero.o_sprite.vel_y;

    // update hero position if map walkable and no collisions
    if (walkable && !level_check_NPC_collid(&p_game->o_level, &p_game->o_hero.o_sprite.bounding_box) && !level_check_chest_collid(&p_game->o_level, &p_game->o_hero.o_sprite.bounding_box))
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

void event_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  p_event->has_triggered = true;
  switch (p_event->o_event_type)
  {
  case EVENT_TYPE_FUNCTION:
  {
    event_callback p_event_callback = (event_callback)p_event->p_param;
    p_event_callback(p_game, renderer);
  }
  break;
  case EVENT_TYPE_TEXT:
    event_text_exec(p_event, p_game, renderer);
    break;
  case EVENT_TYPE_WARP:
    event_warp_exec(p_event, p_game, renderer);
    break;
  case EVENT_TYPE_MONEY:
    event_money_exec(p_event, p_game, renderer);
    break;

  default:
    break;
  }
}

void event_text_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  dialog_box_show(p_game->p_fontmap, (char *)p_event->p_param, SCREEN_HEIGHT, SCREEN_WIDTH, game_draw, p_game, renderer);
}

void event_money_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  int *amount = (int *)p_event->p_param;
  p_game->o_hero.money += *amount;

  char message[32];
  snprintf(message, 32, "YOU GOT %d GOLD COINS!", *amount);

  event o_event;
  event_init(&o_event, ON_TILE_ENTER, EVENT_TYPE_TEXT, message, -1, -1);
  event_text_exec(&o_event, p_game, renderer);
}

void event_warp_exec(event *p_event, game *p_game, SDL_Renderer *renderer)
{
  event_param_warp *p_event_param_warp = (event_param_warp *)p_event->p_param;

  int warp_to_index_x = p_event_param_warp->index_x;
  int warp_to_index_y = p_event_param_warp->index_y;

  if (p_event_param_warp->p_level_filepath)
  {
    char level_pathfile[256];
    strcpy(level_pathfile, p_event_param_warp->p_level_filepath);
    for (int event_index = 0; event_index < p_game->o_level.event_count; event_index++)
    {
      event_free(p_game->o_level.p_event + event_index);
    }
    level_free_partial(&p_game->o_level);
    level_init(&p_game->o_level);

    level_init_from_file(&p_game->o_level, level_pathfile, &p_game->path_music, renderer);

    //initialize NPC and chest
    for (int chest_index = 0; chest_index < p_game->o_level.chest_count; chest_index++)
    {
      // chest tileset is the second tileset in the game
      chest_init(p_game->o_level.p_chest + chest_index, p_game->p_tilesets + 1, renderer);
      p_game->o_level.p_chest[chest_index].is_open = game_get_chest_is_open(p_game, p_game->o_level.p_chest[chest_index].id);
      SDL_Rect bounding_box = {.x = p_game->o_level.p_chest[chest_index].o_sprite.x, .y = p_game->o_level.p_chest[chest_index].o_sprite.y, .w = 16, .h = 16};
      p_game->o_level.p_chest[chest_index].o_sprite.bounding_box = bounding_box;
    }

    for (int NPC_index = 0; NPC_index < p_game->o_level.NPC_count; NPC_index++)
    {
      //NPC tileset is the first tileset in the level
      NPC_init(p_game->o_level.p_NPC + NPC_index, p_game->o_level.p_tileset + 0, renderer);
    }
  }

  p_game->o_hero.o_sprite.x = warp_to_index_x * p_game->o_level.o_tilemap.p_tileset->tile_width;
  p_game->o_hero.o_sprite.y = warp_to_index_y * p_game->o_level.o_tilemap.p_tileset->tile_height;
  game_center_camera_on_hero(p_game);
}

void game_free(game *p_game)
{
  level_free_partial(&p_game->o_level);
  hero_free(&p_game->o_hero);
  image_free(p_game->p_fontmap->p_image);
  free(p_game->path_music);

  for (int tileset_index = 0; tileset_index < p_game->tileset_count; tileset_index++)
  {
    tileset_free(p_game->p_tilesets + tileset_index);
    free(p_game->path_tilesets[tileset_index]);
  }
  free(p_game->p_tilesets);
  free(p_game->path_tilesets);
  free(p_game->o_level.p_tilemap_tileset_path);
}