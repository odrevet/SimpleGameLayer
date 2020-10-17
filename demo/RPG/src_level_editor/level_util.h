#ifndef LEVEL_UTIL_H
#define LEVEL_UTIL_H

#include <stdlib.h>

#include <tilemap.h>

#include "../src/level.h"

bool level_save(level *p_level, const char *pathfile);

void map_add_row(tilemap *map);

void map_add_col(tilemap *map);

void map_shift_down(tilemap *p_map, int from);

void map_shift_right(tilemap *p_map, int from);

#endif