#include "GB.h"

int main(int ac, char **av)
{
	if (ac != 2)
		ERR("Parameter error: ./gb [rom.gb]");
	gb(av[1]);
	return (0);
}
