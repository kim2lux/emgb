#include "rom.h"

int loadRom(char *romfile, struct s_gb *gb)
{
	unsigned int nb_read;

	gb->gb_rom = malloc(sizeof(struct s_rom));
	SDL_RWops *rw = SDL_RWFromFile(romfile, "rb");
	if (rw == NULL)
		DBG("Cannot open rom file");
	gb->gb_rom->size = (unsigned int)SDL_RWsize(rw);
	//printf("rom size = %d\n", s_rom->size);
	gb->gb_rom->rom = malloc(gb->gb_rom->size * sizeof(char));
	if (gb->gb_rom->rom == NULL)
		ERR("Cannot alloc s_rom");
	nb_read = SDL_RWread(rw, gb->gb_rom->rom, sizeof(char), gb->gb_rom->size);
	if (nb_read == gb->gb_rom->size)
		return 0;
	return -1;
}

void loadHeader(struct s_gb *gb)
{
	memcpy(&(gb->gb_rom->romheader), &(gb->gb_rom->rom[0x0100]), HEADER_OFFSET_E - HEADER_OFFSET_S);
}

void displayHeader(struct s_romHeader romheader)
{
	printf("rom name: %s\n", romheader.title);
	printf("cartridge type: %d\n", romheader.cartridgeType);
	printf("rom size: %dKB\n", 32 << romheader.romSize);
	printf("ram size: %d\n", romheader.ramSize << 2);
}

int initRom(char *filename, struct s_gb *gb)
{
	if (loadRom(filename, gb) != 0)
		ERR("error loading rom");
	loadHeader(gb);
	displayHeader(gb->gb_rom->romheader);
	//freeRom(s_rom); woot
	return 0;
}