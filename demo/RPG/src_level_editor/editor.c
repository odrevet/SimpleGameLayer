#include "editor.h"

editor_state editor_edit_layout(editor *p_editor, SDL_Renderer *renderer)
{
    bool done = false;
    editor_state ret_code = QUIT;

    tilemap *p_map = p_editor->p_level->p_map;
    int tileset_nb_tile_x = p_map->p_tileset->p_image->width / p_map->p_tileset->tile_width;

    int scroll_index_x = 0;
    int scroll_index_y = 0;

    while (!done)
    {
        //input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(EXIT_SUCCESS);
            }

            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    done = true;
                    break;
                case SDLK_s:
                    level_save(p_editor->p_level, p_editor->path_level, p_editor->path_tileset, p_editor->path_music);
                    break;
                case SDLK_t:
                    editor_tile_selection(p_editor, renderer);
                    break;
                case SDLK_l:
                    if (p_editor->layer == p_map->nb_layer - 1)
                    {
                        p_editor->layer = 0;
                    }
                    else
                    {
                        p_editor->layer++;
                    }
                    break;
                case SDLK_c:
                    map_add_col(p_map);
                    break;
                case SDLK_r:
                    map_add_row(p_map);
                    break;
                case SDLK_d:
                    map_shift_down(p_map, p_editor->map_tile_index_y);
                    break;
                case SDLK_p:
                    map_shift_right(p_map, p_editor->map_tile_index_y);
                    break;
                case SDLK_i:
                    p_editor->tileset_selected_index = p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].id;
                    break;
                case SDLK_LEFT:
                    if (p_editor->map_tile_index_x > 0)
                    {
                        p_editor->map_tile_index_x--;

                        //update scroll position
                        if (p_editor->map_tile_index_x - scroll_index_x + 1 == 0)
                        {
                            scroll_index_x--;
                            p_map->o_camera.x -= p_map->p_tileset->tile_width;
                        }
                    }

                    break;
                case SDLK_RIGHT:
                    if (p_editor->map_tile_index_x < p_map->width - 1)
                    {
                        p_editor->map_tile_index_x++;

                        //update scroll position
                        if (p_editor->map_tile_index_x >= SCREEN_WIDTH / p_map->p_tileset->tile_width + scroll_index_x)
                        {
                            scroll_index_x++;
                            p_map->o_camera.x += p_map->p_tileset->tile_width;
                        }
                    }
                    break;
                case SDLK_UP:
                    if (p_editor->map_tile_index_y > 0)
                    {
                        p_editor->map_tile_index_y--;

                        //update scroll position
                        if (p_editor->map_tile_index_y - scroll_index_y + 1 == 0)
                        {
                            scroll_index_y--;
                            p_map->o_camera.y -= p_map->p_tileset->tile_height;
                        }
                    }

                    break;
                case SDLK_DOWN:
                    if (p_editor->map_tile_index_y < p_map->height - 1)
                    {
                        p_editor->map_tile_index_y++;
                        //update scroll position
                        if (p_editor->map_tile_index_y >= SCREEN_HEIGHT / p_map->p_tileset->tile_height + scroll_index_y)
                        {
                            scroll_index_y++;
                            p_map->o_camera.y += p_map->p_tileset->tile_height;
                        }
                    }

                    break;
                case SDLK_RETURN:
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].id = p_editor->tileset_selected_index;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.x = (p_editor->tileset_selected_index % tileset_nb_tile_x) * 16;  //TODO tile size from tileset
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.y = (p_editor->tileset_selected_index / tileset_nb_tile_x) * 16;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.h = 16;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.w = 16;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].is_animated = false;
                    break;
                case SDLK_BACKSPACE:
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].id = -1;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.x = 0;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.y = 0;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.h = 0;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].o_frame.w = 0;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].is_animated = false;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            }
        }

        //update animated tile
        for (int animation_index = 0; animation_index < p_map->p_tileset->animation_nb; animation_index++)
        {
            animation_update(p_map->p_tileset->v_animation + animation_index);
        }

        // render
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        map_draw(p_editor->p_level->p_map, renderer);

        // display a rect above the focused tile
        SDL_SetRenderDrawColor(renderer, 250, 100, 100, 200);
        SDL_Rect rect_tile_chipset =
            {.x = p_editor->map_tile_index_x * p_map->p_tileset->tile_width - p_map->o_camera.x,
             .y = p_editor->map_tile_index_y * p_map->p_tileset->tile_height - p_map->o_camera.y,
             .h = p_map->p_tileset->tile_height,
             .w = p_map->p_tileset->tile_width};
        SDL_RenderDrawRect(renderer, &rect_tile_chipset);

        // HUD
        fontmap_printf(p_editor->p_fontmap, 0, 0, renderer, "L %d X %d Y %d ID %d",
                       p_editor->layer,
                       p_editor->map_tile_index_x,
                       p_editor->map_tile_index_y,
                       p_map->p_tiles[p_editor->layer][p_editor->map_tile_index_y][p_editor->map_tile_index_x].id);

        SDL_RenderPresent(renderer);
    }

    return ret_code;
}

editor_state editor_tile_selection(editor *p_editor, SDL_Renderer *renderer)
{
    bool done = false;
    tilemap *p_map = p_editor->p_level->p_map;
    int tileset_nb_tile_y = p_map->p_tileset->p_image->height / p_map->p_tileset->tile_height;
    int tileset_nb_tile_x = p_map->p_tileset->p_image->width / p_map->p_tileset->tile_width;

    // display tileset from index (scroll in the tileset tile selector)
    int scroll_index_x = 0;
    int scroll_index_y = 0;

    while (!done)
    {
        //input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
            {
                exit(EXIT_SUCCESS);
            }

            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    done = true;
                    break;
                case SDLK_s:
                    level_save(p_editor->p_level, p_editor->path_level, p_editor->path_tileset, p_editor->path_music);
                    break;
                case SDLK_LEFT:
                    if (p_editor->tileset_selected_index > 0 && p_editor->tileset_selected_index % tileset_nb_tile_x != 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index--;

                        //update scroll position
                        if (scroll_index_x > p_editor->tileset_selected_index % tileset_nb_tile_x)
                        {
                            scroll_index_x--;
                        }
                    }

                    break;
                case SDLK_RIGHT:
                    if (p_editor->tileset_selected_index < tileset_nb_tile_x * tileset_nb_tile_y && (p_editor->tileset_selected_index + 1) % tileset_nb_tile_x != 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index++;

                        //update scroll position
                        if (p_editor->tileset_selected_index % tileset_nb_tile_x >= scroll_index_x + SCREEN_WIDTH / p_map->p_tileset->tile_width)
                        {
                            scroll_index_x++;
                        }
                    }

                    break;
                case SDLK_UP:
                    if (p_editor->tileset_selected_index - tileset_nb_tile_x >= 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index -= tileset_nb_tile_x;

                        //update scroll position
                        if (scroll_index_y > p_editor->tileset_selected_index / tileset_nb_tile_x)
                        {
                            scroll_index_y--;
                        }
                    }
                    break;
                case SDLK_DOWN:
                    if (p_editor->tileset_selected_index + tileset_nb_tile_x < tileset_nb_tile_x * tileset_nb_tile_y)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index += tileset_nb_tile_x;

                        //update scroll position
                        if (p_editor->tileset_selected_index / tileset_nb_tile_x >= scroll_index_y + SCREEN_HEIGHT / p_map->p_tileset->tile_height)
                        {
                            scroll_index_y++;
                        }
                    }
                    break;
                case SDLK_RETURN:
                    done = true;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            }
        }

        //update display
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        //dislpay the tileset
        SDL_Rect src = {
            .x = scroll_index_x * p_map->p_tileset->tile_width,
            .y = scroll_index_y * p_map->p_tileset->tile_height,
            .w = SCREEN_WIDTH,
            .h = SCREEN_HEIGHT};
        image_draw_part(p_map->p_tileset->p_image, renderer, 0, 0, &src);

        //display a grid
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y < SCREEN_HEIGHT; y += p_map->p_tileset->tile_height)
        {
            for (int x = 0; x < SCREEN_WIDTH; x += p_map->p_tileset->tile_width)
            {
                SDL_RenderDrawLine(renderer,
                                   0,
                                   y,
                                   SCREEN_WIDTH,
                                   y);

                //draw a vertical grid line
                SDL_RenderDrawLine(renderer,
                                   x,
                                   0,
                                   x,
                                   SCREEN_HEIGHT);
            }
        }

        //display a rect above the focused tile in the tileset
        SDL_SetRenderDrawColor(renderer, 250, 100, 100, 200);
        SDL_Rect rect_tile_chipset =
            {.x = ((p_editor->tileset_selected_index % tileset_nb_tile_x) * p_map->p_tileset->tile_width) - scroll_index_x * p_map->p_tileset->tile_width,
             .y = ((p_editor->tileset_selected_index / tileset_nb_tile_x) * p_map->p_tileset->tile_height) - scroll_index_y * p_map->p_tileset->tile_height,
             .h = p_map->p_tileset->tile_height,
             .w = p_map->p_tileset->tile_width};
        SDL_RenderDrawRect(renderer, &rect_tile_chipset);

        // HUD
        fontmap_printf(p_editor->p_fontmap, 0, 0, renderer, "%d", p_editor->tileset_selected_index);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
