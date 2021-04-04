#include "editor.h"

void editor_init(editor *p_editor)
{
    p_editor->tileset_selected_is_animated = false;
    p_editor->tileset_selected_index = 0;
    p_editor->tileset_selected_animated_index = 0;
    p_editor->layer = 0;
    p_editor->map_tile_index.x = 0;
    p_editor->map_tile_index.y = 0;
    p_editor->map_tile_index.w = 1;
    p_editor->map_tile_index.h = 1;
    p_editor->path_level = NULL;
    p_editor->p_tileset = NULL;
    p_editor->path_music = NULL;
    p_editor->tile_select_scroll_index_x = 0;
    p_editor->tile_select_scroll_index_y = 0;
    level_init(&p_editor->o_level);
}

void set_tile(editor *p_editor, int index_x, int index_y)
{
    tilemap *p_map = &p_editor->o_level.o_tilemap;
    int tileset_nb_tile_x = p_map->p_tileset->o_image.width / p_map->p_tileset->tile_width;

    if (p_editor->tileset_selected_is_animated)
    {
        p_map->p_tiles[p_editor->layer][index_y][index_x].is_animated = true;
        p_map->p_tiles[p_editor->layer][index_y][index_x].id = p_editor->tileset_selected_animated_index;
        p_map->p_tiles[p_editor->layer][index_y][index_x].p_animation = p_editor->o_level.o_tilemap.p_tileset->v_animation + p_editor->tileset_selected_animated_index;
    }
    else
    {
        p_map->p_tiles[p_editor->layer][index_y][index_x].id = p_editor->tileset_selected_index;
        p_map->p_tiles[p_editor->layer][index_y][index_x].is_animated = false;
        animation_set_frame(&p_map->p_tiles[p_editor->layer][index_y][index_x].o_frame,
                            (p_editor->tileset_selected_index % tileset_nb_tile_x) * p_editor->o_level.o_tilemap.p_tileset->tile_width,
                            (p_editor->tileset_selected_index / tileset_nb_tile_x) * p_editor->o_level.o_tilemap.p_tileset->tile_width,
                            p_editor->o_level.o_tilemap.p_tileset->tile_height,
                            p_editor->o_level.o_tilemap.p_tileset->tile_width);
    }
}

editor_state editor_edit_layout(editor *p_editor, SDL_Renderer *renderer)
{
    bool done = false;

    tilemap *p_map = &p_editor->o_level.o_tilemap;
    int tileset_nb_tile_x = p_map->p_tileset->o_image.width / p_map->p_tileset->tile_width;

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
                    level_map_save(&p_editor->o_level, p_editor->path_level);
                    break;
                case SDLK_t:
                    editor_tile_selection(p_editor, renderer);
                    break;
                case SDLK_a:
                    if (p_map->p_tileset->animation_nb > 0)
                    {
                        editor_animated_tile_selection(p_editor, renderer);
                    }
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
                    map_shift_down(p_map, p_editor->map_tile_index.y);
                    break;
                case SDLK_p:
                    map_shift_right(p_map, p_editor->map_tile_index.y);
                    break;
                case SDLK_i:
                    p_editor->tileset_selected_index = p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].id;
                    break;
                case SDLK_LEFT:
                    if (p_editor->map_tile_index.x > 0)
                    {
                        p_editor->map_tile_index.x--;

                        //update scroll position
                        if (p_editor->map_tile_index.x - scroll_index_x + 1 == 0)
                        {
                            scroll_index_x--;
                            p_map->o_camera.x -= p_map->p_tileset->tile_width;
                        }
                    }

                    break;
                case SDLK_RIGHT:
                    if (p_editor->map_tile_index.x < p_map->width - 1)
                    {
                        if (event.key.keysym.mod & KMOD_SHIFT)
                        {
                            p_editor->map_tile_index.w++;
                        }
                        else
                        {
                            p_editor->map_tile_index.x++;
                        }
                        //update scroll position
                        if (p_editor->map_tile_index.x >= SCREEN_WIDTH / p_map->p_tileset->tile_width + scroll_index_x)
                        {
                            scroll_index_x++;
                            p_map->o_camera.x += p_map->p_tileset->tile_width;
                        }
                    }
                    break;
                case SDLK_UP:
                    if (p_editor->map_tile_index.y > 0)
                    {
                        p_editor->map_tile_index.y--;

                        //update scroll position
                        if (p_editor->map_tile_index.y - scroll_index_y + 1 == 0)
                        {
                            scroll_index_y--;
                            p_map->o_camera.y -= p_map->p_tileset->tile_height;
                        }
                    }

                    break;
                case SDLK_DOWN:
                    if (p_editor->map_tile_index.y < p_map->height - 1)
                    {
                        if (event.key.keysym.mod & KMOD_SHIFT)
                        {
                            p_editor->map_tile_index.h++;
                        }
                        else
                        {
                            p_editor->map_tile_index.y++;
                        }

                        //update scroll position
                        if (p_editor->map_tile_index.y >= SCREEN_HEIGHT / p_map->p_tileset->tile_height + scroll_index_y)
                        {
                            scroll_index_y++;
                            p_map->o_camera.y += p_map->p_tileset->tile_height;
                        }
                    }

                    break;
                case SDLK_RETURN:
                    for (int index_x = p_editor->map_tile_index.x; index_x < p_editor->map_tile_index.x + p_editor->map_tile_index.w; index_x++)
                    {
                        for (int index_y = p_editor->map_tile_index.y; index_y < p_editor->map_tile_index.y + p_editor->map_tile_index.h; index_y++)
                        {
                            set_tile(p_editor, index_x, index_y);
                        }
                    }
                    break;
                case SDLK_BACKSPACE:
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].id = -1;
                    p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].is_animated = false;
                    animation_set_frame(&p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].o_frame, 0, 0, 0, 0);
                    break;
                case SDLK_ESCAPE:
                    p_editor->map_tile_index.w = 1;
                    p_editor->map_tile_index.h = 1;
                    break;
                default:
                    break;
                }
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

        map_draw(&p_editor->o_level.o_tilemap, renderer);

        // display a rect above the focused tile
        SDL_SetRenderDrawColor(renderer, 250, 100, 100, 200);
        SDL_Rect rect_tile_chipset =
            {.x = p_editor->map_tile_index.x * p_map->p_tileset->tile_width - p_map->o_camera.x,
             .y = p_editor->map_tile_index.y * p_map->p_tileset->tile_height - p_map->o_camera.y,
             .h = p_editor->map_tile_index.h * p_map->p_tileset->tile_height,
             .w = p_editor->map_tile_index.w * p_map->p_tileset->tile_width};
        SDL_RenderDrawRect(renderer, &rect_tile_chipset);

        // HUD
        fontmap_printf(p_editor->p_fontmap, 0, 0, renderer, "L %d X %d Y %d ID %d %s",
                       p_editor->layer,
                       p_editor->map_tile_index.x,
                       p_editor->map_tile_index.y,
                       p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].id,
                       p_map->p_tiles[p_editor->layer][p_editor->map_tile_index.y][p_editor->map_tile_index.x].is_animated ? "ANIMATED" : "");

        SDL_RenderPresent(renderer);
    }

    return QUIT;
}

editor_state editor_tile_selection(editor *p_editor, SDL_Renderer *renderer)
{
    bool done = false;
    tileset *p_tileset = p_editor->p_tileset;
    int tileset_nb_tile_y = p_tileset->o_image.height / p_tileset->tile_height;
    int tileset_nb_tile_x = p_tileset->o_image.width / p_tileset->tile_width;

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
                case SDLK_LEFT:
                    if (p_editor->tileset_selected_index > 0 && p_editor->tileset_selected_index % tileset_nb_tile_x != 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index--;

                        //update scroll position
                        if (p_editor->tile_select_scroll_index_x > p_editor->tileset_selected_index % tileset_nb_tile_x)
                        {
                            p_editor->tile_select_scroll_index_x--;
                        }
                    }

                    break;
                case SDLK_RIGHT:
                    if (p_editor->tileset_selected_index < tileset_nb_tile_x * tileset_nb_tile_y && (p_editor->tileset_selected_index + 1) % tileset_nb_tile_x != 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index++;

                        //update scroll position
                        if (p_editor->tileset_selected_index % tileset_nb_tile_x >= p_editor->tile_select_scroll_index_x + SCREEN_WIDTH / p_tileset->tile_width)
                        {
                            p_editor->tile_select_scroll_index_x++;
                        }
                    }

                    break;
                case SDLK_UP:
                    if (p_editor->tileset_selected_index - tileset_nb_tile_x >= 0)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index -= tileset_nb_tile_x;

                        //update scroll position
                        if (p_editor->tile_select_scroll_index_y > p_editor->tileset_selected_index / tileset_nb_tile_x)
                        {
                            p_editor->tile_select_scroll_index_y--;
                        }
                    }
                    break;
                case SDLK_DOWN:
                    if (p_editor->tileset_selected_index + tileset_nb_tile_x < tileset_nb_tile_x * tileset_nb_tile_y)
                    {
                        //update selected tile index
                        p_editor->tileset_selected_index += tileset_nb_tile_x;

                        //update scroll position
                        if (p_editor->tileset_selected_index / tileset_nb_tile_x >= p_editor->tile_select_scroll_index_y + SCREEN_HEIGHT / p_tileset->tile_height)
                        {
                            p_editor->tile_select_scroll_index_y++;
                        }
                    }
                    break;
                case SDLK_RETURN:
                    p_editor->tileset_selected_is_animated = false;
                    done = true;
                    break;
                default:
                    break;
                }
                break;
            }
        }

        //update display
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        //dislpay the tileset
        SDL_Rect src = {
            .x = p_editor->tile_select_scroll_index_x * p_tileset->tile_width,
            .y = p_editor->tile_select_scroll_index_y * p_tileset->tile_height,
            .w = p_tileset->o_image.width < SCREEN_WIDTH ? p_tileset->o_image.width : SCREEN_WIDTH,
            .h = p_tileset->o_image.height < SCREEN_HEIGHT ? p_tileset->o_image.height : SCREEN_HEIGHT};
        image_draw_part(&p_tileset->o_image, renderer, 0, 0, &src);

        //display a grid
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        editor_tileset_render_grid(p_tileset, renderer);

        //display a rect above the focused tile in the tileset
        SDL_SetRenderDrawColor(renderer, 250, 100, 100, 200);
        SDL_Rect rect_tile_chipset =
            {.x = ((p_editor->tileset_selected_index % tileset_nb_tile_x) * p_tileset->tile_width) - p_editor->tile_select_scroll_index_x * p_tileset->tile_width,
             .y = ((p_editor->tileset_selected_index / tileset_nb_tile_x) * p_tileset->tile_height) - p_editor->tile_select_scroll_index_y * p_tileset->tile_height,
             .h = p_tileset->tile_height,
             .w = p_tileset->tile_width};
        SDL_RenderDrawRect(renderer, &rect_tile_chipset);

        // HUD
        fontmap_printf(p_editor->p_fontmap, 0, 0, renderer, "%d", p_editor->tileset_selected_index);

        SDL_RenderPresent(renderer);
    }

    return QUIT;
}

editor_state editor_animated_tile_selection(editor *p_editor, SDL_Renderer *renderer)
{
    bool done = false;
    tileset *p_tileset = p_editor->p_tileset;

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
                case SDLK_UP:
                    if (p_editor->tileset_selected_animated_index > 0)
                    {
                        p_editor->tileset_selected_animated_index--;
                    }
                    break;
                case SDLK_DOWN:
                    if (p_editor->tileset_selected_animated_index + 1 < p_tileset->animation_nb)
                    {
                        p_editor->tileset_selected_animated_index++;
                    }
                    break;
                case SDLK_RETURN:
                    p_editor->tileset_selected_is_animated = true;
                    done = true;
                    break;
                default:
                    break;
                }
                break;
            }
        }

        //update animated tile
        for (int animation_index = 0; animation_index < p_tileset->animation_nb; animation_index++)
        {
            animation_update(p_tileset->v_animation + animation_index);
        }

        // update display
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        editor_render_tileset_animations(p_tileset, renderer);

        // display a grid
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        editor_tileset_render_grid(p_tileset, renderer);

        // display a rect above the selected animated tile
        SDL_SetRenderDrawColor(renderer, 250, 100, 100, 200);
        SDL_Rect rect_tile_animated =
            {.x = 0,
             .y = p_editor->tileset_selected_animated_index * p_tileset->tile_height,
             .h = p_tileset->tile_height,
             .w = p_tileset->tile_width};
        SDL_RenderDrawRect(renderer, &rect_tile_animated);

        // HUD
        fontmap_printf(p_editor->p_fontmap, 0, 0, renderer, "%d", p_editor->tileset_selected_index);

        SDL_RenderPresent(renderer);
    }

    return QUIT;
}

void editor_render_tileset_animations(tileset *p_tileset, SDL_Renderer *renderer)
{
    // display all animated tiles
    for (int animation_index = 0; animation_index < p_tileset->animation_nb; animation_index++)
    {
        // display the animation on the first column
        animation *p_animation = p_tileset->v_animation + animation_index;
        int y = animation_index * p_tileset->tile_height;
        int frame_current = p_animation->frame_current;
        SDL_Rect src = p_animation->v_frame[frame_current];
        image_draw_part(&p_tileset->o_image, renderer, 0, y, &src);

        // display all frames of the animation
        for (int frame_index = 0; frame_index < p_tileset->animation_nb; frame_index++)
        {
            SDL_Rect *src = p_animation->v_frame + frame_index;
            image_draw_part(&p_tileset->o_image, renderer, (frame_index + 1) * p_tileset->tile_width, y, src);
        }
    }
}

void editor_tileset_render_grid(tileset *p_tileset, SDL_Renderer *renderer)
{
    for (int y = 0; y < SCREEN_HEIGHT; y += p_tileset->tile_height)
    {
        for (int x = 0; x < SCREEN_WIDTH; x += p_tileset->tile_width)
        {
            //draw horizontal line
            SDL_RenderDrawLine(renderer,
                               0,
                               y,
                               SCREEN_WIDTH,
                               y);

            //draw vertical line
            SDL_RenderDrawLine(renderer,
                               x,
                               0,
                               x,
                               SCREEN_HEIGHT);
        }
    }
}