#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include <SDL.h>

#include <init.h>
#include <image.h>
#include <fontmap.h>
#include <timer.h>

#include "globals.h"
#include "game.h"
#include "level.h"
#include "shapes.h"

game_state state_in_game(SDL_Renderer *renderer, game *p_game);
game_state state_paused(SDL_Renderer *renderer);
game_state state_game_over(SDL_Renderer *renderer, game *p_game);

void blink(char *p_level, SDL_Renderer *renderer, int *lines, int line_nb, image *p_image_block, image *p_image_background);

int WINDOW_HEIGHT = 480;
int WINDOW_WIDTH = 640;

int SCREEN_WIDTH = 280;
int SCREEN_HEIGHT = 240;

SDL_Joystick *joystick;
int joystick_repeat_delay;

int main(int argc, char **argv)
{
	int flags = 0;
	//parse command line arguments
	int c;
	while ((c = getopt(argc, argv, "f")) != -1)
	{
		switch (c)
		{
		case 'f':
			flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;
		default:
			abort();
		}
	}

	srand(time(NULL));
	init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	atexit(SDL_Quit);
	SDL_ShowCursor(SDL_DISABLE);

	// create a new window
	SDL_Window *window = create_window("Tetris",
									   &WINDOW_WIDTH,
									   &WINDOW_HEIGHT,
									   flags);

	//create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window,
												0,
												SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	game o_game;
	fontmap o_fontmap;
	o_fontmap.character_size = 7;  //size in pixels for a character

	// number of characters in the layout
	o_fontmap.layout = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!-";
	image image_fontmap;
	image_load(&image_fontmap, "res/font.png", renderer, NULL);
	o_fontmap.p_image = &image_fontmap;
	o_game.p_fontmap = &o_fontmap;

	game_state ret_code = IN_GAME;
	while (ret_code != QUIT)
	{
		switch (ret_code)
		{
		case IN_GAME:
			ret_code = state_in_game(renderer, &o_game);
			break;
		case GAME_OVER:
			ret_code = state_game_over(renderer, &o_game);
			break;
		default:
			break;
		}
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}

game_state state_in_game(SDL_Renderer *renderer, game *p_game)
{
	image o_image_block;
	image_load(&o_image_block, "res/gfx.png", renderer, NULL);

	image o_image_background;
	image_load(&o_image_background, "res/bg.png", renderer, NULL);

	bool done = false;
	game_state ret_code = GAME_OVER;
	long timelastcall = SDL_GetTicks();

	//initialize the states of completed lines to 0
	memset(p_game->state,
		   0,
		   STATE_NUM * sizeof(int));
	p_game->score = 0;

	int level_cur = 0;
	int level_max = 10;

	level_init(p_game->level);

	//declare two shapes, one controled by the player, the other to indicates the next shape
	shape o_falling_shape, o_preview_shape;

	//set the falling shape type randomly
	shape_set(&o_falling_shape, rand() % SHAPE_NB);
	shape_default_coord(&o_falling_shape);

	shape_type next_shape_type = rand() % SHAPE_NB;
	shape_set(&o_preview_shape, next_shape_type);

	//put the preview shape on the side of the board
	o_preview_shape.x = LEVEL_WIDTH + 1;
	o_preview_shape.y = o_preview_shape.len - SHAPE_SIZE / 2 + 2;

	while (!done)
	{
		//drawing
		SDL_SetRenderDrawColor(renderer, 0, 42, 0, 255);
		SDL_RenderClear(renderer);

		//background image
		image_draw(&o_image_background, renderer, 0, 0);

		//draw the falling shape
		shape_draw(&o_falling_shape, &o_image_block, renderer);

		//draw the next shape as a preview
		shape_draw(&o_preview_shape, &o_image_block, renderer);

		//draw the level (empty blocks and alderly placed shapes)
		level_draw(p_game->level, renderer, &o_falling_shape, &o_image_block);

		int game_speed = level_max * 100 - level_cur * 100;

		int total = 0;
		for (int i = 0; i < 3; i++)
			total += p_game->state[i];

		//draw a line between the board and the score / next shape area
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawLine(renderer,
						   TILE_SIZE * 12,
						   0,
						   TILE_SIZE * 12,
						   SCREEN_HEIGHT);

		//draw the score
		fontmap_printf(p_game->p_fontmap,
					   SCREEN_WIDTH - 7 * 5,
					   SCREEN_HEIGHT - 120,
					   renderer,
					   "%d",
					   p_game->score);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{

			if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) ||
				(event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 5))
			{
				ret_code = state_paused(renderer);
				if (ret_code != STAY)
				{
					done = 1;
				}
			}

			if (event.type == SDL_QUIT)
			{
				exit(EXIT_SUCCESS);
			}

			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RIGHT:
				  if (shape_move(p_game->level, &o_falling_shape, 1, 0))
						o_falling_shape.x++;
					break;
				case SDLK_LEFT:
				  if (shape_move(p_game->level, &o_falling_shape, -1, 0))
						o_falling_shape.x--;
					break;
				case SDLK_DOWN:
				  if (shape_move(p_game->level, &o_falling_shape, 0, 1))
					{
						o_falling_shape.y++;
						timelastcall = SDL_GetTicks() + game_speed;
					}
					break;
				case SDLK_SPACE:
				  while (shape_move(p_game->level, &o_falling_shape, 0, 1))
						o_falling_shape.y++;
					timelastcall = SDL_GetTicks() + game_speed;
					break;
				case SDLK_f:
					shape_rotate(p_game->level, &o_falling_shape, 0);
					break;
				case SDLK_d:
					shape_rotate(p_game->level, &o_falling_shape, 1);
					break;
				case SDLK_q:
					exit(EXIT_SUCCESS);
					break;
				default:
					break;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				switch (event.jbutton.button)
				{
				case 0:
				  while (shape_move(p_game->level, &o_falling_shape, 0, 1))
						o_falling_shape.y++;
					timelastcall = SDL_GetTicks() + game_speed;
					break;
				case 2:
					shape_rotate(p_game->level, &o_falling_shape, 1);
					break;
				case 3:
					shape_rotate(p_game->level, &o_falling_shape, 0);
					break;
				case 6:
					done = true;
					ret_code = QUIT;
					break;
				case 19:
					done = true;
					ret_code = QUIT;
					break;
				default:
					break;
				}
				break;
			}
		}

		if (SDL_GetTicks() - timelastcall > game_speed)
		{
			//check if the falling shape can move
		  if (shape_move(p_game->level, &o_falling_shape, 0, 1))
			{
				//update coord
				o_falling_shape.y++;
			}
			else
			{
				//the shape can not move
				int i;
				level_add_shape(p_game->level, &o_falling_shape);

				int lines_in_a_row = 0;
				//tab of line index to remove
				int rem_tab[SHAPE_SIZE];
				int line_nb = level_check_line(p_game->level, rem_tab);

				for (i = 1; i < line_nb; i++)
				{
					if (rem_tab[i] == rem_tab[i - 1] + 1)
						lines_in_a_row++;
					else
						lines_in_a_row = 0;
				}

				if (line_nb)
				{
					//blink completed line(s)
				  blink(p_game->level, renderer, rem_tab, line_nb, &o_image_block, &o_image_background);

					for (i = 0; i < line_nb; i++)
					  level_remove_line(p_game->level, rem_tab[i]);				 //remove completed line(s)
					p_game->score += line_nb * (lines_in_a_row + 1); //update score
					p_game->state[lines_in_a_row]++;				 //update game states
					if (line_nb == 2 && lines_in_a_row == 0)
						p_game->state[0]++;

					level_cur = p_game->score / 1000; // update level number
					if (level_cur >= level_max)
						level_cur = level_max;
				}

				if (level_check_game_over(p_game->level))
				{
					done = 1;
					ret_code = GAME_OVER;
				}
				else
				{
					//"create" a new falling block by chaning the falling shape type
					//to the next shape and reseting the coords
					shape_set(&o_falling_shape, next_shape_type);
					shape_default_coord(&o_falling_shape);

					//pick a next shape type randomly for the preview and the next falling
					//block
					next_shape_type = rand() % SHAPE_NB;
					shape_set(&o_preview_shape, next_shape_type);
				}
			}

			//update timer
			timelastcall = SDL_GetTicks();
		}

		SDL_RenderPresent(renderer);
	}

	return ret_code;
}

game_state state_paused(SDL_Renderer *renderer)
{
	game_state ret_code = STAY;
	bool done = false;
	while (!done)
	{
		SDL_Event event;
		SDL_JoystickUpdate();
		while (SDL_PollEvent(&event))
		{
			if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) ||
				(event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 5))
			{
				ret_code = STAY;
				done = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_r:
					ret_code = IN_GAME;
					done = true;
					break;
				case SDLK_o:
					ret_code = QUIT;
					done = true;
					break;
				default:
					break;
				}
			}
			else if (event.type == SDL_JOYBUTTONDOWN)
			{
				switch (event.jbutton.button)
				{
				case 0:
					ret_code = STAY;
					done = true;
					break;
				case 1:
					ret_code = IN_GAME;
					done = true;
					break;
				case 6:
					done = true;
					ret_code = QUIT;
					break;
				case 19:
					done = true;
					ret_code = QUIT;
					break;
				default:
					break;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				exit(EXIT_SUCCESS);
			}
		}
	}
	return ret_code;
}

game_state state_game_over(SDL_Renderer *renderer, game *p_game)
{
	bool done = false;
	timer o_timer;
	timer_init(&o_timer);
	timer_start(&o_timer);

	int total = 0;
	for (int i = 0; i < 3; i++)
		total += p_game->state[i];

	while (!done)
	{
		if (timer_get_ticks(&o_timer) >= 5 * 1000)
		{
			done = true;
		}

		SDL_Event event;
		SDL_JoystickUpdate();
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) ||
				(event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 5))
			{
				done = true;
			}
		}

		//print the score
		fontmap_printf(p_game->p_fontmap,
					   WINDOW_WIDTH / 2,
					   WINDOW_HEIGHT / 2,
					   renderer,
					   "%d",
					   p_game->score);

		SDL_RenderPresent(renderer);
	}

	return IN_GAME;
}

void blink(char *p_level, SDL_Renderer *renderer, int *lines, int line_nb, image *p_image_block, image *p_image_background)
{
	bool done = false;
	timer o_timer;
	timer_init(&o_timer);
	timer_start(&o_timer);

	//number of times the lines must blink
	int blink_time = 5;

	//number of times the lines has blinked
	int blink_cur = 0;

	//time in milliseconds of the blink speed
	int blink_duration = 200;

	//show full line or empty line
	char show = 1;
	int i;

	while (!done)
	{
		if (timer_get_ticks(&o_timer) >= blink_duration)
		{
			if (blink_cur == blink_time)
				done = true;
			else
			{
				show = (blink_cur % 2 == 0) ? 1 : 0;
				for (i = 0; i < line_nb; i++)
				{
					memset(p_level + lines[i], show, sizeof(char) * LEVEL_WIDTH);
				}
				blink_cur++;
				o_timer.start_ticks = SDL_GetTicks();
			}
		}

		//draw the level
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 42, 0, 255);
		image_draw(p_image_background, renderer, 0, 0);
		level_draw(p_level, renderer, NULL, p_image_block);

		SDL_RenderPresent(renderer);
	}
}
