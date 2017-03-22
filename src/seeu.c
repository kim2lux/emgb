#include "GB.h"

void	seeu(struct s_gb *s_gb)
{
	SDL_DestroyWindow(s_gb->gb_gpu.window_d);
	SDL_DestroyWindow(s_gb->gb_gpu.window);
	free(s_gb->gb_rom->rom);
	free(s_gb->gb_rom);
	free(s_gb->gb_gpu.pixels_d);
	free(s_gb->gb_gpu.pixels);
	free(s_gb);

	SDL_Quit();
}