//#include "io.h"
#include "GB.h"



void	updateLcdc(struct s_gb *s_gb)
{
	if (s_gb->gb_io.lcdc & 0x01) s_gb->gb_io.lcd.BgWindowDisplay = 1;
	else s_gb->gb_io.lcd.BgWindowDisplay = 0;

	if (s_gb->gb_io.lcdc & 0x02) s_gb->gb_io.lcd.SpriteIsOn = 1;
	else s_gb->gb_io.lcd.SpriteIsOn = 0;

	if (s_gb->gb_io.lcdc & 0x04) s_gb->gb_io.lcd.SpriteSize = 16;
	else s_gb->gb_io.lcd.SpriteSize = 8;

	if (s_gb->gb_io.lcdc & 0x08) s_gb->gb_io.lcd.BgTileMapSelect = 0x9c00;
	else s_gb->gb_io.lcd.BgTileMapSelect = 0x9800;

	if (s_gb->gb_io.lcdc & 0x10) s_gb->gb_io.lcd.BgWindowTileData = 0x8000;
	else s_gb->gb_io.lcd.BgWindowTileData = 0x8800;

	if (s_gb->gb_io.lcdc & 0x20) s_gb->gb_io.lcd.WindowIsOn = 1;
	else s_gb->gb_io.lcd.WindowIsOn = 0;

	if (s_gb->gb_io.lcdc & 0x40) s_gb->gb_io.lcd.WindowTileMapSelect = 0x9c00;
	else s_gb->gb_io.lcd.WindowTileMapSelect = 0x9800;

	if (s_gb->gb_io.lcdc & 0x80) s_gb->gb_io.lcd.LcdIsOn = 1;
	else s_gb->gb_io.lcd.LcdIsOn = 0;

	printf(" s_gb->gb_io.lcd.BgTileMapSelect %x s_gb->gb_io.lcd.BgWindowTileData %x\n", s_gb->gb_io.lcd.BgTileMapSelect, s_gb->gb_io.lcd.BgWindowTileData);
	printf("display windows ? %x\n", s_gb->gb_io.lcd.WindowIsOn);

}

unsigned char padState(struct s_gb *s_gb)
{
	if ((s_gb->gb_pad.key & 0x20) == 0)
		return (0xc0 | s_gb->gb_pad.button_key | 0x10);
	else if ((s_gb->gb_pad.key & 0x10) == 0)
		return (0xc0 | s_gb->gb_pad.button_dir | 0x20);
	return (0xff);
}

void oamTransfert(unsigned char src, struct s_gb *s_gb)
{
	unsigned short oamsrc  = (src << 8);

	for (int pos = 0; pos <= 160; pos++)
	{
		s_gb->gb_mem.oam_[pos] = read8bit(oamsrc + pos);
	}
}

void ctrlIo(unsigned short addr, unsigned char *io_ports, struct s_gb *s_gb)
{
	switch (addr) {
	case 0xff00:
	{
		s_gb->gb_pad.key = io_ports[0x00];
		break;
	}
	case 0xff01:
	case 0xff02:
	{
		s_gb->gb_io.serial = io_ports[0x01]; break;
		break;
	}
	case 0xff04:
	{
		//divide counter
		break;
	}
	case 0xff05: {
		break;
	}
	case 0xff06: {
		break;

	}
	case 0xff07: {
		s_gb->gb_io.timerCtrl = io_ports[0x07];
		initTimer(s_gb);
		printf("timer ctrl %x\n", s_gb->gb_io.timerCtrl); break;
	}
	case 0xff24: {
		s_gb->gb_io.voice = io_ports[0x24]; break;
	}
	case 0xff25: {
		s_gb->gb_io.sndStereo = io_ports[0x25]; break;
	}
	case 0xff26: {
		s_gb->gb_io.sndStat = io_ports[0x26]; break;
	}
	case 0xff0f: {
		s_gb->gb_interrupts.interFlag = io_ports[0x0f];
		break;
	}
	case 0xff40: {
		s_gb->gb_io.lcdc = io_ports[0x40];
		updateLcdc(s_gb);
		break;
	}
	case 0xff42: //scroll Y
	{
		s_gb->gb_io.scrollY = io_ports[0x42];
		break;
	}
	case 0xff43: //scroll Y
	{
		s_gb->gb_io.scrollX = io_ports[0x43]; break;
		printf("scroll x = %x\n", s_gb->gb_io.scrollX);
	}
	case 0xff46: //oam
	{
		oamTransfert(io_ports[0x46], s_gb);
		break;
	}
	case 0xff4A: //scroll Y
	{
		printf("win y = %x\n", io_ports[0x4A]);
		break;
	}
	case 0xff4B: //scroll Y
	{
		printf("win x = %x\n", io_ports[0x4B]);
		break;
	}

	}
	return;
}
