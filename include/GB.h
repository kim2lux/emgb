#ifndef __GB__
#define __GB__

#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "cpu.h"
#include "rom.h"
#include "interupt.h"
#include "gpu.h"
#include "io.h"
#include "joypad.h"
#include "timer.h"
#include "memory.h"



struct						s_gb
{
	unsigned char			running;
	struct s_timer			gb_time;
	struct s_register		gb_register;
	struct s_io				gb_io;
	struct s_rom			*gb_rom;
	struct s_gpu			gb_gpu;
	struct s_interupt		gb_interrupts;
	struct s_memory			gb_mem;
	struct s_joypad			gb_pad;
	struct s_cpu			gb_cpu;
};

void	initRegister(struct s_gb *s_gb);
void	gb(char *fileName);
void	debug(struct s_rom *tmprom);
void	check_neg(unsigned char value, struct s_gb *s_gb);
void	check_zero(unsigned char value, struct s_gb *s_gb);
void	RDBG(struct s_gb *s_gb);
void	displayStack(struct s_gb *s_gb);

#define CLOCKSPEED 4194304



FILE *fp;

#define ERR(s)  {printf("ERR:: %s ---- %s line %d\n", s, __FILE__, __LINE__); exit(0); }
#define DBG(s)	{printf("DBG:: %s ---- %s line %d\n", s, __FILE__, __LINE__); }
#define SUCC(s) printf("OK:: %s\n", s);

enum FLAG { ZERO, NEG, HALF, CARRY };

//cb
void	handleCB(unsigned char cbopcode, struct s_gb *);


#endif