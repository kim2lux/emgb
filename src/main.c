#include "GB.h"


int main(int ac, char **av)
{
	if (ac == 1)
	{
		char * mario = "C:\\proj\\GB_test\\emgb\\roms\\mario.gb";
		char * tetris = "C:\\proj\\GB_test\\emgb\\roms\\Tetris.gb";
		char * special = "C:\\proj\\GB_test\\emgb\\roms\\01-special.gb";
		{
			char * rompath = tetris;
			gb(rompath);
		}
	}
	else
		gb(av[1]);
	return (0);
}
