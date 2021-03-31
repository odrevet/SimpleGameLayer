#include "dialog.h"

void dialog_draw(dialog *p_dialog, SDL_Renderer *renderer)
{
	//bckground
	SDL_SetRenderDrawColor(renderer, p_dialog->o_style.bgcolor.r, p_dialog->o_style.bgcolor.g, p_dialog->o_style.bgcolor.b, p_dialog->o_style.bgcolor.a);
	SDL_Rect rect = {.x = p_dialog->o_style.x, .y = p_dialog->o_style.y, .h = p_dialog->o_style.h, .w = p_dialog->o_style.w};
	SDL_RenderFillRect(renderer, &rect);

	//border
	SDL_SetRenderDrawColor(renderer, p_dialog->o_style.border.r, p_dialog->o_style.border.g, p_dialog->o_style.border.b, p_dialog->o_style.border.a);
	SDL_RenderDrawRect(renderer, &rect);

	//text
	fontmap_print(p_dialog->p_fontmap, p_dialog->o_style.x + p_dialog->o_style.padding, p_dialog->o_style.y + p_dialog->o_style.padding, renderer, p_dialog->text);

	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}
