#include "level.h"

void level_init(level *p_level)
{
  p_level->path_tile_property = NULL;
  p_level->p_music = NULL;
  p_level->p_tile_properties = NULL;
  p_level->p_event = NULL;
  p_level->event_count = 0;
  p_level->p_NPC = NULL;
  p_level->NPC_count = 0;
  map_init(p_level->p_map);
}

bool level_load(level *p_level, const char *pathfile, char **current_path_tileset, char **current_path_music, SDL_Renderer *renderer)
{
  tilemap *p_map = p_level->p_map;

  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    return false;
  }

  char buffer[256];

  // tileset
  fscanf(fp, "%s", buffer);
  if (*current_path_tileset == NULL || strcmp(buffer, *current_path_tileset) != 0)
  {
    *current_path_tileset = realloc(*current_path_tileset, strlen(buffer) * sizeof(char));
    strcpy(*current_path_tileset, buffer);
    image_free(p_map->p_tileset->p_image);
    image_load(p_map->p_tileset->p_image, *current_path_tileset, renderer, NULL);
  }

  // tile properties
  fscanf(fp, "%s", buffer);
  p_level->path_tile_property = realloc(p_level->path_tile_property, strlen(buffer) * sizeof(char));
  strcpy(p_level->path_tile_property, buffer);
  tile_property *v_tile_property = NULL;
  int nb_tile_property;
  v_tile_property = level_parse_tiles_file(p_level, buffer, &nb_tile_property, v_tile_property);

  // music
  fscanf(fp, "%s", buffer);
  if (*current_path_music == NULL || strcmp(buffer, *current_path_music) != 0)
  {
    *current_path_music = realloc(*current_path_music , strlen(buffer) * sizeof(char));
    strcpy(*current_path_music, buffer);
    p_level->p_music = music_load(*current_path_music);
    music_play(p_level->p_music);
  }

  // line break
  fgetc(fp);

  // map tile size
  fscanf(fp, "%d:%d", &p_map->p_tileset->tile_width, &p_map->p_tileset->tile_height);

  // map width height and nb of layers
  fscanf(fp, "%d:%d:%d", &p_map->width, &p_map->height, &p_map->nb_layer);

  // line break
  fgetc(fp);

  map_tiles_alloc(p_map);

  // allocate tile properties
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
          p_tile->p_animation = p_level->p_map->p_tileset->v_animation + tile_index;
        }
        else
        {
          p_tile->is_animated = false;
          tile_index = atoi(tile_idstr);
          p_tile->o_frame.x = tile_index % (p_map->p_tileset->p_image->width / p_map->p_tileset->tile_width) * p_map->p_tileset->tile_width;
          p_tile->o_frame.y = tile_index / (p_map->p_tileset->p_image->width / p_map->p_tileset->tile_width) * p_map->p_tileset->tile_height;
          p_tile->o_frame.w = p_map->p_tileset->tile_width;
          p_tile->o_frame.h = p_map->p_tileset->tile_height;
        }

        p_tile->id = tile_index;

        // tile property
        tile_property *p_tile_property = p_level->p_tile_properties[index_layer][index_height] + index_width;
        p_tile_property->id = tile_index;

        for (int index_property = 0; index_property < nb_tile_property; index_property++)
        {
          if (v_tile_property[index_property].id == tile_index)
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

tile_property *level_parse_tiles_file(level *p_level, const char *pathfile, int *nb_tile_property, tile_property *v_tile_property)
{
  FILE *fp = fopen(pathfile, "r");
  if (!fp)
  {
    return false;
  }

  // tiles animated
  int nb_tile_animated;

  fscanf(fp, "%d", &nb_tile_animated);

  // line break
  fgetc(fp);

  p_level->p_map->p_tileset->animation_nb = nb_tile_animated;

  if (nb_tile_animated > 0)
  {
    p_level->p_map->p_tileset->v_animation = calloc(nb_tile_animated, sizeof(animation));
    for (int index_tile = 0; index_tile < nb_tile_animated; index_tile++)
    {
      int nb_frame;
      fscanf(fp, "%d", &nb_frame);
      animation_init(p_level->p_map->p_tileset->v_animation + index_tile, true, nb_frame, 10, 0);
      for (int index_frame = 0; index_frame < nb_frame; index_frame++)
      {
        int tile_index;
        fscanf(fp, "%d", &tile_index);

        int x = tile_index % (p_level->p_map->p_tileset->p_image->width / 16 /*p_map->p_tileset->tile_width*/); //TODO parse width before tile
        int y = tile_index / (p_level->p_map->p_tileset->p_image->width / 16 /*p_map->p_tileset->tile_width*/);
        animation_set_frame(p_level->p_map->p_tileset->v_animation[index_tile].v_frame + index_frame, x * 16, y * 16, 16, 16);
      }
    }
  }
  // tile properties
  int nb_properties_per_tile;

  fscanf(fp, "%d %d", nb_tile_property, &nb_properties_per_tile);

  // line break
  fgetc(fp);

  v_tile_property = calloc(*nb_tile_property, sizeof(tile_property));

  for (int index_tile = 0; index_tile < *nb_tile_property; index_tile++)
  {
    int tile_id;
    char properties[nb_properties_per_tile + 1];

    fscanf(fp, "%d %s", &tile_id, properties);

    v_tile_property[index_tile].id = tile_id;
    v_tile_property[index_tile].walkable = properties[0] != '0';

    // line break
    fgetc(fp);
  }

  fclose(fp);

  return v_tile_property;
}

void level_free(level *p_level)
{
  //free tiles properties
  for (int index_layer = 0; index_layer < p_level->p_map->nb_layer; index_layer++)
  {
    for (int index_height = 0; index_height < p_level->p_map->height; index_height++)
    {
      free(p_level->p_tile_properties[index_layer][index_height]);
    }
    free(p_level->p_tile_properties[index_layer]);
  }

  free(p_level->p_tile_properties);
  map_tiles_free(p_level->p_map);

  for (int index_NPC = 0; index_NPC < p_level->NPC_count; index_NPC++)
  {
    NPC_free(p_level->p_NPC + index_NPC);
  }
  free(p_level->p_NPC);
  free(p_level->p_event);
  free(p_level->path_tile_property);
}