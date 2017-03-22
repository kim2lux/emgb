#include "interupt.h"
#include "GB.h"

void vblanck(struct s_gb * gb_s)
{
	gb_s->gb_cpu.totalTick += 12;
	gb_s->gb_interrupts.interMaster = 0;
	gb_s->gb_register.sp -= 2;
	write16bitToAddr(gb_s->gb_register.sp, gb_s->gb_register.pc, gb_s);
	gb_s->gb_register.pc = 0x40;

}

void lcd(struct s_gb * gb_s)
{
	gb_s->gb_cpu.totalTick += 12;
	gb_s->gb_interrupts.interMaster = 0;
	printf("doing lcdc !\n");
	gb_s->gb_register.sp -= 2;
	write16bitToAddr(gb_s->gb_register.sp, gb_s->gb_register.pc, gb_s);
	gb_s->gb_register.pc = 0x48;
}

void joypad(struct s_gb * gb_s)
{
	gb_s->gb_interrupts.interMaster = 0;
	gb_s->gb_cpu.totalTick += 12;
	gb_s->gb_register.sp -= 2;
	write16bitToAddr(gb_s->gb_register.sp, gb_s->gb_register.pc, gb_s);
	gb_s->gb_register.pc = 0x60;
}

void serial(struct s_gb * gb_s)
{
	gb_s->gb_interrupts.interMaster = 0;
	gb_s->gb_cpu.totalTick += 12;
	gb_s->gb_register.sp -= 2;
	write16bitToAddr(gb_s->gb_register.sp, gb_s->gb_register.pc, gb_s);
	gb_s->gb_register.pc = 0x58;
}

void timer(struct s_gb * gb_s)
{
	debug(gb_s);
	gb_s->gb_interrupts.interMaster = 0;
	gb_s->gb_cpu.totalTick += 12;
	gb_s->gb_register.sp -= 2;
	write16bitToAddr(gb_s->gb_register.sp, gb_s->gb_register.pc, gb_s);
	gb_s->gb_register.pc = 0x50;
}

void doInterupt(struct s_gb * gb_s)
{
//	printf("inter master %x interenable %x inter flag %x\n", gb_s->gb_interrupts.interMaster, gb_s->gb_interrupts.interEnable, gb_s->gb_interrupts.interFlag);
	if (gb_s->gb_interrupts.interMaster && gb_s->gb_interrupts.interEnable && gb_s->gb_interrupts.interFlag)
	{
		//printf("\ninterupt !\n");
		gb_s->gb_cpu.stopCpu = 0;
		unsigned char inter = gb_s->gb_interrupts.interEnable & gb_s->gb_interrupts.interFlag;
		//getchar();
		if (inter & INT_VBLANK) {
			gb_s->gb_interrupts.interFlag &= ~(INT_VBLANK);
			vblanck(gb_s);
		}
		if (inter & INT_LCDSTAT) {
			{
				printf("LCD interupt\n");
				gb_s->gb_interrupts.interFlag &= ~(INT_LCDSTAT);
				lcd(gb_s);
			}
		}
		if (inter & INT_TIMER) {
			timer(gb_s);
			gb_s->gb_interrupts.interFlag &= ~(INT_TIMER);
		}
		if (inter & INT_JOYPAD) {
			printf("JOYPAD interupt\n");
			joypad(gb_s);
			gb_s->gb_interrupts.interFlag &= ~(INT_JOYPAD);
		}
		if (inter & INT_SERIAL) {
			printf("serial interupt\n");
			serial(gb_s);
			gb_s->gb_interrupts.interFlag &= ~(INT_SERIAL);
		}
		return;
	}
}
