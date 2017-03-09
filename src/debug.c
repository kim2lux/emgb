#include "GB.h"

void	displayStack(struct s_gb *s_gb)
{
	unsigned short value;

	value = s_gb->gb_register.sp + 10;
	while (value >= s_gb->gb_register.sp)
	{
		printf("sp <%x> value <%x>\n", value, read16bit(value, s_gb));
		value -= 2;
	}
}

void check_neg(unsigned char value, struct s_gb *s_gb)
{
	if (value == 0)
		SET_NEG();
	else
		CLEAR_NEG();
}

void check_zero(unsigned char value, struct s_gb *s_gb)
{
	if (value == 0)
		SET_ZERO();
	else
		CLEAR_ZERO();
}

void	RDBG(struct s_gb *s_gb)
{
	printf("----------\npc value : %s pc: %x\nsp: %x\nopcode: %x\n----------\n", 
		s_gb->gb_cpu.gb_cpu_z80[read8bit(s_gb->gb_register.pc, s_gb)].value, s_gb->gb_register.pc, s_gb->gb_register.sp, read8bit(s_gb->gb_register.pc, s_gb));

	printf("register a %x f %x af -- %x\n", s_gb->gb_register.a, s_gb->gb_register.f, s_gb->gb_register.af);
	printf("register b %x c %x cb -- %x\n", s_gb->gb_register.b, s_gb->gb_register.c, s_gb->gb_register.bc);
	printf("register d %x e %x de -- %x\n", s_gb->gb_register.d, s_gb->gb_register.e, s_gb->gb_register.de);
	printf("register h %x l %x hl -- %x\n", s_gb->gb_register.h, s_gb->gb_register.l, s_gb->gb_register.hl);
	printf("flag Zero %x\n", s_gb->gb_register.f >> 7);
	printf("flag Substraction %x\n", (s_gb->gb_register.f >> 6) & 1);
	printf("flag Half-carry %x\n", (s_gb->gb_register.f >> 5) & 1);
	printf("flag  Carry %x\n", (s_gb->gb_register.f >> 4) & 1);

	displayStack(s_gb);
	printf("----end-----");
	getchar();
}

void debug(struct s_rom *tmprom)
{
  printf("%d\n", tmprom->size);
}
