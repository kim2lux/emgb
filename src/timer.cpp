#include "timer.h"
#include "GB.h"

void initTimer(struct s_gb *s_gb)
{
	unsigned char TMC = read8bit(0xFF07, s_gb);
	printf("tmc value %d\n", TMC);
	if ((TMC & 0x3) == 0)
		s_gb->gb_time.freq = 4096;
	else if ((TMC & 0x3) == 1)
		s_gb->gb_time.freq = 262144;
	else if ((TMC & 0x3) == 2)
		s_gb->gb_time.freq = 65536;
	else if ((TMC & 0x3) == 3)
		s_gb->gb_time.freq = 16384;
	s_gb->gb_time.timerCount = CLOCKSPEED / s_gb->gb_time.freq;

}

void updateTimer(struct s_gb *s_gb)
{
	static  int lastTick = 0;
	unsigned char TMC = read8bit(0xFF07, s_gb);

	if (TMC & 4) //is timer enable ?
	{
		s_gb->gb_time.timerCount -= (s_gb->gb_cpu.totalTick - lastTick); //FOR TEST !!! - lastTick;
		lastTick = s_gb->gb_cpu.totalTick;

		if (s_gb->gb_time.timerCount <= 0)
		{
			s_gb->gb_time.timerCount = CLOCKSPEED / s_gb->gb_time.freq;
			if (read8bit(0xff05, s_gb) == 255)
			{
				write8bit(0xff05, read8bit(0xff06, s_gb), s_gb);
				s_gb->gb_interrupts.interFlag |= INT_TIMER;
			}
			else
				write8bit(0xff05, read8bit(0xff05, s_gb) + 1, s_gb);
		}
	}
}
