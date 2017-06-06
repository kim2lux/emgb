#include "GB.h"

#define IMDBG
// main loop function
// retrieve opcode & execute it. update gpu interupt & timer
int imgui(void *s_gb);

void gb(char *fileName)
{
	unsigned char	fopcode = 0;
	struct s_gb		*s_gb = NULL;
	SDL_Thread		*thr = NULL;

	s_gb = initGb(fileName);
	s_gb->stopdbg = 0;
#ifdef IMDBG
	s_gb->stopdbg = 0;
	thr = SDL_CreateThread(imgui, "dbg", s_gb);
	if (thr == NULL)
	{
		printf("cannot start imGui dbg\n");
	}
#endif
	while (s_gb->running)
	{
		;
		debug(s_gb);
		handleEvent(s_gb);
		if (s_gb->gb_cpu.stopCpu == 0) 
		{
			fopcode = read8bit(s_gb->gb_register.pc, s_gb); s_gb->gb_register.pc += 1; //retrieve func opcode
			s_gb->gb_cpu.gb_cpu_z80[fopcode].func(s_gb); //call opcode func pointer
			if (s_gb->gb_cpu.jmpf == 0) // if jmp opcode was called, no need to incr PC
				s_gb->gb_register.pc += s_gb->gb_cpu.gb_cpu_z80[fopcode].size;
			s_gb->gb_cpu.jmpf = 0;
		}
		updateGpu(s_gb);
		doInterupt(s_gb);
		updateTimer(s_gb);
	}
#ifdef IMDBG
	if (thr != NULL)
	{
		s_gb->stopdbg = 1;
		printf("waiting dbg thread to exit\n");
		SDL_WaitThread(thr, NULL);
	}
#endif
	seeu(s_gb);
}
