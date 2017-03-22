#include "GB.h"

struct s_gb	*initGb(char *fileName)
{
	struct s_gb		*s_gb = NULL;

	s_gb = malloc(sizeof(*s_gb));
	if (s_gb == NULL)
		ERR("Cannot allocate s_gb");
	initRom(fileName, s_gb);
	initRegister(s_gb);
	initDisplay(s_gb);
	initGpu(s_gb);
	initTimer(s_gb);
	initCpu(s_gb);
	return (s_gb);
}

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
