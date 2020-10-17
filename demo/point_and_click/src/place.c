#include "place.h"

void place_init(place *p_place)
{
  p_place->v_image = NULL;
  p_place->p_images_loc = NULL;
  p_place->image_count = 0;
  p_place->p_hotspots = NULL;
  p_place->hotspot_count = 0;
}

void place_init_from_file(place *p_place, char *filepath, SDL_Renderer *renderer)
{
#ifdef DEBUG
  printf("LOADING PLACE %s\n", filepath);
#endif

  FILE *fp = fopen(filepath, "r");

  if (fp == NULL)
  {
    printf("CANNOT LOAD FILE %s\n", filepath);
    exit(0);
  }

  int image_count;
  fscanf(fp, "%d", &image_count);
  p_place->image_count = image_count;
  p_place->v_image = calloc(sizeof(image *), image_count);

  for (int i = 0; i < image_count; i++)
  {
    char image_path[256];
    fscanf(fp, "%s", image_path);
    image_load(p_place->v_image, image_path, renderer, NULL);
  }

  int hotspot_count;
  fscanf(fp, "%d", &hotspot_count);

  p_place->p_hotspots = malloc(hotspot_count * sizeof(hotspot));

  for (int i = 0; i < hotspot_count; i++)
  {
    SDL_Rect hs_loc;
    fscanf(fp, "%d %d %d %d", &hs_loc.x, &hs_loc.y, &hs_loc.w, &hs_loc.h);

#ifdef DEBUG
    printf("Hotspot location at %d %d %d %d\n", hs_loc.x, hs_loc.y, hs_loc.w, hs_loc.h);
#endif

    p_place->hotspot_count = hotspot_count;
    hotspot *p_hotspot = p_place->p_hotspots + i;

    p_hotspot->location = hs_loc;
    p_hotspot->on_click = NULL;

    //read type character
    char type[1];
    fscanf(fp, "%s", type);

#ifdef DEBUG
    printf("TYPE IS %s\n", type);
#endif

    if (strcmp(type, "L") == 0)
    {
      p_hotspot->type = HS_LINK;
      char destination_id[8];
      fscanf(fp, "%s", destination_id);

#ifdef DEBUG
      printf("DESTINATION ID IS %s\n", destination_id);
#endif

      p_hotspot->param = malloc(strlen(destination_id) * sizeof(char));
      strcpy(p_hotspot->param, destination_id);
      p_hotspot->on_click = callback_link;
    }
    else if (strcmp(type, "F") == 0)
    {
      p_hotspot->type = HS_LINK;

      int index;
      fscanf(fp, "%d", &index);

#ifdef DEBUG
      printf("function index is %d\n", index);
#endif

      p_hotspot->param = NULL;
      if (index >= 0 && index < CALLBACK_USER_COUNT)
      {
        p_hotspot->on_click = *(callbacks_user + index);
      }
    }
    else if (strcmp(type, "T") == 0)
    {
      p_hotspot->type = HS_TEXT;

      char text[BUFSIZ];
      fscanf(fp, "%[^\n]", text);

      p_hotspot->param = text;
#ifdef DEBUG
      printf("text is %s\n", text);
#endif
      p_hotspot->on_click = callback_text;
    }
    else
    {
      printf("WARNING: UNKNOWN HOTSPOT TYPE %s IN %s\n", type, filepath);
    }
  }
  fclose(fp);
}

void place_draw(place *p_place, SDL_Renderer *renderer)
{
  for (int i = 0; i < p_place->image_count; i++)
  {
    image_draw(p_place->v_image + i, renderer, 0, 0);
  }

#ifdef DEBUG
  //display location of each hotspot locations
  for (int i = 0; i < p_place->hotspot_count; i++)
  {
    SDL_RenderDrawRect(renderer, &p_place->p_hotspots[i].location);
  }
#endif
}

void place_free(place *p_place)
{
  for (int i = 0; i < p_place->image_count; i++)
  {
    image_free(p_place->v_image + i);
  }
  free(p_place->p_hotspots);
}
