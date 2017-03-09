#ifndef IO_HH
#define IO_HH

struct s_gb;
void ctrlIo(unsigned short addr, unsigned char *io_ports, struct s_gb *s_gb);
void oamTransfert(unsigned char src, struct s_gb *s_gb);
unsigned char padState(struct s_gb *s_gb);
void	updateLcdc(struct s_gb *s_gb);
void memoryInit(struct s_gb *s_gb);

struct lcd
{
	unsigned char LcdIsOn;
	unsigned short WindowTileMapSelect;
	unsigned short BgTileMapSelect;
	unsigned char WindowIsOn;
	unsigned short BgWindowTileData;
	unsigned char  SpriteSize;
	unsigned char  SpriteIsOn;
	unsigned char  BgWindowDisplay;
};

struct s_io
{
	unsigned char scrollX;
	unsigned char scrollY;
	unsigned char winX;
	unsigned char winY;
	unsigned char serial;
	unsigned char timerCtrl;
	unsigned char sndStat;
	unsigned char sndStereo;
	unsigned char voice;
	unsigned char lcdc;

	struct lcd  lcd;
};



#endif
