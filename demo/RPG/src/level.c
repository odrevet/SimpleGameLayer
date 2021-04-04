#include "level.h"

void level_init(level *p_level)
{
  p_level->path_tile_property = NULL;
  p_level->p_music = NULL;
  p_level->p_tile_properties = NULL;
  p_level->p_event = NULL;
  p_level->event_count = 0;
  p_level->NPC_count = 0;
  p_level->p_NPC = NULL;
  p_level->chest_count = 0;
  p_level->p_chest = 0;
}

bool level_check_NPC_collid(level *p_level, SDL_Rect *p_bounding_box)
{
  for (int index_NPC = 0; index_NPC < p_level->NPC_count; index_NPC++)
  {
    NPC *p_NPC = p_level->p_NPC + index_NPC;
    if (SDL_HasIntersection(&p_NPC->o_sprite.bounding_box, p_bounding_box))
    {
      return true;
    }
  }
  return false;
}

bool level_check_chest_collid(level *p_level, SDL_Rect *p_bounding_box)
{
  for (int index_chest = 0; index_chest < p_level->chest_count; index_chest++)
  {
    chest *p_chest = p_level->p_chest + index_chest;
    if (SDL_HasIntersection(&p_chest->o_sprite.bounding_box, p_bounding_box))
    {
      return true;
    }
  }
  return false;
}

bool level_map_init_from_file(level *p_level, const char *pathfile, SDL_Renderer *renderer)
{
  tilemap *p_map = &p_level->o_tilemap;

  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    return false;
  }

  char buffer[256];

  // tileset check with p_tilemap_tileset_path if alderly loaded
  fscanf(fp, "%s", buffer);
  if (!p_level->p_tilemap_tileset_path || strcmp(buffer, p_level->p_tilemap_tileset_path) != 0)
  {
    if (p_map->p_tileset)
    {
      tileset_free(p_map->p_tileset);
    }
    free(p_map->p_tileset);
    p_map->p_tileset = malloc(sizeof(tileset));
    tileset_init_from_file(p_map->p_tileset, buffer, renderer);
    p_level->p_tilemap_tileset_path = realloc(p_level->p_tilemap_tileset_path, (strlen(buffer) + 1) * sizeof(char));
    strcpy(p_level->p_tilemap_tileset_path, buffer);
  }

  // tile properties path
  fscanf(fp, "%s", buffer);
  p_level->path_tile_property = realloc(p_level->path_tile_property, (strlen(buffer) + 1) * sizeof(char));
  strcpy(p_level->path_tile_property, buffer);

  // map width height and nb of layers
  fscanf(fp, "%d:%d:%d", &p_map->width, &p_map->height, &p_map->nb_layer);

  // allocate tiles
  map_tiles_alloc(p_map);

  // allocate tile properties
  tile_property *v_tile_property = NULL;
  int nb_tile_property;
  v_tile_property = level_tile_props_init_from_file(p_level, buffer, &nb_tile_property, v_tile_property);

  p_level->p_tile_properties = calloc(p_map->nb_layer, sizeof(tile_property **));
  for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
  {
    p_level->p_tile_properties[index_layer] = calloc(p_map->height, sizeof(tile_property *));
    for (int index_height = 0; index_height < p_map->height; index_height++)
    {
      p_level->p_tile_properties[index_layer][index_height] = calloc(p_map->width, sizeof(tile_property));
    }
  }

  for (int index_layer = 0; index_layer < p_map->nb_layer; index_layer++)
  {
    for (int index_height = 0; index_height < p_map->height; index_height++)
    {
      for (int index_width = 0; index_width < p_map->width; index_width++)
      {
        tile *p_tile = p_map->p_tiles[index_layer][index_height] + index_width;

        int tile_index;
        char tile_idstr[8];
        fscanf(fp, "%s", tile_idstr);

        if (strcmp(tile_idstr, "_") == 0)
        {
          // blank tile
          tile_index = -1;
        }
        else if (tile_idstr[0] == 'a')
        {
          p_tile->is_animated = true;
          char *p_tile_idstr = tile_idstr;
          tile_index = atoi(++p_tile_idstr);
          p_tile->p_animation = p_level->o_tilemap.p_tileset->v_animation + tile_index;
        }
        else
        {
          p_tile->is_animated = false;
          tile_index = atoi(tile_idstr);
          p_tile->o_frame.x = tile_index % (p_map->p_tileset->o_image.width / p_map->p_tileset->tile_width) * p_map->p_tileset->tile_width;
          p_tile->o_frame.y = tile_index / (p_map->p_tileset->o_image.width / p_map->p_tileset->tile_width) * p_map->p_tileset->tile_height;
          p_tile->o_frame.w = p_map->p_tileset->tile_width;
          p_tile->o_frame.h = p_map->p_tileset->tile_height;
        }

        p_tile->id = tile_index;

        // tile property
        tile_property *p_tile_property = p_level->p_tile_properties[index_layer][index_height] + index_width;
        p_tile_property->id = p_tile->id;
        p_tile_property->is_animated = p_tile->is_animated;

        for (int index_property = 0; index_property < nb_tile_property; index_property++)
        {
          if (v_tile_property[index_property].id == p_tile->id && p_tile_property->is_animated == v_tile_property[index_property].is_animated)
          {
            p_tile_property->walkable = v_tile_property[index_property].walkable;
            break;
          }
        }
      }
    }
  }

  fclose(fp);

  return true;
}

tile_property *level_tile_props_init_from_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property)
{
  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    return false;
  }

  int nb_properties_per_tile;

  fscanf(fp, "%d %d", nb_tile_property, &nb_properties_per_tile);

  // line break
  fgetc(fp);

  v_tile_property = calloc(*nb_tile_property, sizeof(tile_property));

  for (int index_tile = 0; index_tile < *nb_tile_property; index_tile++)
  {
    int tile_id;
    char tile_idstr[8];
    char properties[nb_properties_per_tile + 1];

    fscanf(fp, "%s %s", tile_idstr, properties);

    if (tile_idstr[0] == 'a')
    {
      v_tile_property[index_tile].is_animated = true;
      char *p_tile_idstr = tile_idstr;
      tile_id = atoi(++p_tile_idstr);
    }
    else
    {
      v_tile_property[index_tile].is_animated = false;
      tile_id = atoi(tile_idstr);
    }

    v_tile_property[index_tile].id = tile_id;
    v_tile_property[index_tile].walkable = properties[0] != '0';

    // line break
    fgetc(fp);
  }

  fclose(fp);

  return v_tile_property;
}

bool level_init_from_file(level *p_level, char *pathfile, char **current_path_music, SDL_Renderer *renderer)
{
  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    printf("cannot found level '%s'\n", pathfile);
    return false;
  }

  char buffer[256];

  // to be assigned to level
  int tileset_count;
  char **p_tileset_path;
  tileset *p_tileset;

  fscanf(fp, "%d", &tileset_count);

  p_tileset_path = calloc(tileset_count, sizeof(char *));
  p_tileset = calloc(tileset_count, sizeof(tileset));

  // read tileset paths
  for (int tileset_index = 0; tileset_index < tileset_count; tileset_index++)
  {
    fscanf(fp, "%s", buffer);
    p_tileset_path[tileset_index] = calloc(strlen(buffer) + 1, sizeof(char));
    strcpy(p_tileset_path[tileset_index], buffer);
  }

  // intersection of path of tileset alderly loaded
  int p_level_tileset_to_keep_indexes[p_level->tileset_count];
  int level_tileset_to_keep_indexes_count = 0;

  for (int tileset_index = 0; tileset_index < tileset_count; tileset_index++)
  {
    bool alderly_loaded = false;
    int level_tileset_index;

    for (level_tileset_index = 0; level_tileset_index < p_level->tileset_count; level_tileset_index++)
    {
      if (strcmp(p_tileset_path[tileset_index], p_level->p_tileset_path[level_tileset_index]) == 0)
      {
        alderly_loaded = true;
        p_level_tileset_to_keep_indexes[level_tileset_to_keep_indexes_count] = level_tileset_index;
        level_tileset_to_keep_indexes_count++;
        break;
      }
    }

    if (alderly_loaded == false)
    {
      tileset_init_from_file(p_tileset + tileset_index, p_tileset_path[tileset_index], renderer);
    }
    else
    {
      p_tileset[tileset_index] = p_level->p_tileset[level_tileset_index];
    }
  }

  // free tilesets we dont need anymore
  for (int level_tileset_index = 0; level_tileset_index < p_level->tileset_count; level_tileset_index++)
  {
    bool keep = false;
    for (int tileset_index = 0; tileset_index < level_tileset_to_keep_indexes_count; tileset_index++)
    {
      if (p_level_tileset_to_keep_indexes[tileset_index] == level_tileset_index)
      {
        keep = true;
        break;
      }
    }

    if (!keep)
    {
      tileset_free(p_level->p_tileset + level_tileset_index);
    }
  }
  free(p_level->p_tileset);

  // free previous level tileset path
  for (int level_tileset_path_index = 0; level_tileset_path_index < p_level->tileset_count; level_tileset_path_index++)
  {
    free(p_level->p_tileset_path[level_tileset_path_index]);
  }
  free(p_level->p_tileset_path);

  // assign to level
  p_level->tileset_count = tileset_count;
  p_level->p_tileset_path = p_tileset_path;
  p_level->p_tileset = p_tileset;

  // music
  fscanf(fp, "%s", buffer);
  if (*current_path_music == NULL || strcmp(buffer, *current_path_music) != 0)
  {
    *current_path_music = realloc(*current_path_music, (strlen(buffer) + 1) * sizeof(char));
    strcpy(*current_path_music, buffer);
    p_level->p_music = music_load(*current_path_music);
    music_play(p_level->p_music);
  }

  // Events
  fscanf(fp, "%d", &p_level->event_count);
  p_level->p_event = calloc(p_level->event_count, sizeof(event));
  for (int event_index = 0; event_index < p_level->event_count; event_index++)
  {
    fscanf(fp, "%s", buffer);
    if (strcmp(buffer, "on_tile_enter") == 0)
    {
      p_level->p_event[event_index].o_event_trigger = ON_TILE_ENTER;
    }
    else if (strcmp(buffer, "on_button_press") == 0)
    {
      p_level->p_event[event_index].o_event_trigger = ON_BUTTON_PRESS;
    }
    else
    {
      printf("UNKNOWN EVENT TYPE %s\n", buffer);
    }

    fscanf(fp, "%d %d", &p_level->p_event[event_index].index_src_x, &p_level->p_event[event_index].index_src_y);

    fscanf(fp, "%s", buffer);
    if (strcmp(buffer, "warp") == 0)
    {
      p_level->p_event[event_index].o_event_type = EVENT_TYPE_WARP;
      int dest_x, dest_y, function_index;
      fscanf(fp, "%d %d %d", &dest_x, &dest_y, &function_index);
      event_param_warp *p_event_param_warp = malloc(sizeof(event_param_warp));
      p_event_param_warp->index_x = dest_x;
      p_event_param_warp->index_y = dest_y;
      p_event_param_warp->p_level_addr = *(p_level->callbacks + function_index);
      p_level->p_event[event_index].p_param = p_event_param_warp;
    }
    else if (strcmp(buffer, "text") == 0)
    {
      p_level->p_event[event_index].o_event_type = EVENT_TYPE_TEXT;
      fscanf(fp, " \"%[^\"]\"", buffer);
      p_level->p_event[event_index].p_param = calloc(strlen(buffer) + 1, sizeof(char));
      strcpy(p_level->p_event[event_index].p_param, buffer);
    }
    else if (strcmp(buffer, "money") == 0)
    {
      p_level->p_event[event_index].o_event_type = EVENT_TYPE_MONEY;
      int *amount = malloc(sizeof(int));
      fscanf(fp, "%d", amount);
      p_level->p_event[event_index].p_param = amount;
    }
    else if (strcmp(buffer, "function") == 0)
    {
      p_level->p_event[event_index].o_event_type = EVENT_TYPE_FUNCTION;
      int function_index;
      fscanf(fp, "%d", &function_index);
      p_level->p_event[event_index].p_param = *(p_level->callbacks + function_index);
    }
  }

  // NPCs
  fscanf(fp, "%d", &p_level->NPC_count);
  p_level->p_NPC = calloc(p_level->NPC_count, sizeof(NPC));
  for (int NPC_index = 0; NPC_index < p_level->NPC_count; NPC_index++)
  {
    int event_index;
    fscanf(fp, "%d", &event_index);
    p_level->p_NPC[NPC_index].p_event = p_level->p_event + event_index;
    fscanf(fp, "%f %f", &p_level->p_NPC[NPC_index].o_sprite.x, &p_level->p_NPC[NPC_index].o_sprite.y);
    p_level->p_NPC[NPC_index].o_sprite.x *= p_level->o_tilemap.p_tileset->tile_width;
    p_level->p_NPC[NPC_index].o_sprite.y *= p_level->o_tilemap.p_tileset->tile_height;
  }

  // Chests
  fscanf(fp, "%d", &p_level->chest_count);
  p_level->p_chest = calloc(p_level->chest_count, sizeof(chest));
  for (int chest_index = 0; chest_index < p_level->chest_count; chest_index++)
  {
    int event_index;
    fscanf(fp, "%d", &event_index);
    p_level->p_chest[chest_index].p_event = p_level->p_event + event_index;
    fscanf(fp, "%f %f", &p_level->p_chest[chest_index].o_sprite.x, &p_level->p_chest[chest_index].o_sprite.y);
    p_level->p_chest[chest_index].o_sprite.x *= p_level->o_tilemap.p_tileset->tile_width;
    p_level->p_chest[chest_index].o_sprite.y *= p_level->o_tilemap.p_tileset->tile_height;
  }

  fclose(fp);
  return true;
}

void level_free_partial(level *p_level)
{
  //free tiles properties
  if (p_level->p_tile_properties)
  {
    for (int index_layer = 0; index_layer < p_level->o_tilemap.nb_layer; index_layer++)
    {
      for (int index_height = 0; index_height < p_level->o_tilemap.height; index_height++)
      {
        free(p_level->p_tile_properties[index_layer][index_height]);
      }
      free(p_level->p_tile_properties[index_layer]);
    }

    free(p_level->p_tile_properties);
  }
  if (p_level->o_tilemap.p_tiles)
  {
    map_tiles_free(&p_level->o_tilemap);
  }

  free(p_level->p_NPC);
  free(p_level->p_event);
  free(p_level->path_tile_property);
}