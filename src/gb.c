#include "GB.h"

void	initRegister(struct s_gb *s_gb)
{
	s_gb->gb_register.af = 0x01B0;
	s_gb->gb_register.bc = 0x0013;
	s_gb->gb_register.de = 0x00D8;
	s_gb->gb_register.hl = 0x014D;
	s_gb->gb_register.pc = 0x0100;
	s_gb->gb_register.sp = 0xFFFA;

	s_gb->gb_cpu.stopCpu = 0;
	s_gb->gb_cpu.totalTick = 0;
	s_gb->gb_cpu.jmpf = 0;

	s_gb->gb_gpu.gpuMode = HBLANK;

	memoryInit(s_gb);

	
	s_gb->gb_pad.button_key = 0x0f;
	s_gb->gb_pad.button_dir = 0x0f;

	s_gb->running = 1;
	SET_ZERO();
	SET_HALFC();
	SET_CARRY();
	CLEAR_NEG();
}

// main loop function
// retrieve opcode & execute it. update gpu interupt & timer

void gb(char *fileName)
{
	unsigned char	fopcode = 0;
	struct s_gb		*s_gb;
	
	s_gb = malloc(sizeof(*s_gb));
	initRom(fileName, s_gb);
	initRegister(s_gb);
	initDisplay(s_gb);
	initGpu(s_gb);
	initTimer(s_gb);
	initCpu(s_gb);

	while (s_gb->running)
	{
		handleEvent(s_gb);
		if (s_gb->gb_cpu.stopCpu == 0) {
			fopcode = read8bit(s_gb->gb_register.pc, s_gb); s_gb->gb_register.pc += 1; //retrieve func opcode
			s_gb->gb_cpu.gb_cpu_z80[fopcode].func(s_gb); //call opcode func pointer
			if (s_gb->gb_cpu.jmpf == 0) // Did I jmp ?
				s_gb->gb_register.pc += s_gb->gb_cpu.gb_cpu_z80[fopcode].size;
			s_gb->gb_cpu.jmpf = 0;
		}
		updateGpu(s_gb);
		doInterupt(s_gb);
		updateTimer(s_gb);
	}

	SDL_DestroyWindow(s_gb->gb_gpu.window);
	SDL_DestroyWindow(s_gb->gb_gpu.window_d);

	free(s_gb->gb_rom->rom);
	free(s_gb->gb_rom);
	free(s_gb->gb_gpu.pixels);
	free(s_gb);

	SDL_Quit();
	exit(0);
}
