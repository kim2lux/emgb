#ifndef __MEMORY__
#define __MEMORY__

int write8bit(unsigned short addr, unsigned char value, struct s_gb *s_gb);
unsigned char memoperation(unsigned short addr, signed char value, struct s_gb *s_gb);
void	mcbHandleBanking(unsigned short addr, unsigned char value, struct s_gb *s_gb);
unsigned char read8bit(unsigned short addr, struct s_gb *s_gb);
unsigned short read16bit(unsigned short addr, struct s_gb *s_gb);
void write16bitToAddr(unsigned short addr, unsigned short value, struct s_gb *s_gb);
void call_addr_16(struct s_gb *s_gb);


struct						s_memory 
{
	unsigned char			sram[0x2000];
	unsigned char			vram[0x2000];
	unsigned char			ram[0x2000];
	unsigned char			oam[0x00FF];
	unsigned char			io_ports[0x80];
	unsigned char			hram[0x80];
};

#endif
