#include "GB.h"

// main loop function
// retrieve opcode & execute it. update gpu interupt & timer

void gb(char *fileName)
{
	unsigned char	fopcode = 0;
	struct s_gb		*s_gb = NULL;

	s_gb = initGb(fileName);
	while (s_gb->running)
	{
		debug(s_gb);
		handleEvent(s_gb);
		if (s_gb->gb_cpu.stopCpu == 0) 
		{
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
	seeu(s_gb);
}
