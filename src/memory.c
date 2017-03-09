#include "GB.h"
#include "memory.h"
static unsigned char MCB_romBanking = 1;
static unsigned char romBankingFlag = 0;

void write16bitToAddr(unsigned short addr, unsigned short value, struct s_gb *s_gb)
{
	write8bit(addr, (unsigned char)value & 0x00ff, s_gb);
	write8bit(addr + 1, (unsigned char)((value & 0xff00) >> 8), s_gb);
}

unsigned short read16bit(unsigned short addr, struct s_gb *s_gb)
{
	unsigned short res = 0;

	res |= read8bit(addr, s_gb);
	res |= read8bit(addr + 1, s_gb) << 8;

	return res;
}

unsigned char read8bit(unsigned short addr, struct s_gb *s_gb)
{

	if (addr == 0xff44)
	{
		return s_gb->gb_gpu.scanline;
	}
	else if (addr < 0x4000)
	{
		return (s_gb->gb_rom->rom[addr]);
	}
	else if (addr >= 0x4000 && addr < 0x8000)
	{
		//printf("MCB_romBanking value = %x\n", MCB_romBanking);
		return (s_gb->gb_rom->rom[(addr - 0x4000) + (MCB_romBanking*0x4000)]);
	}
	else if (addr >= 0x8000 && addr < 0xA000)
	{
		return s_gb->gb_mem.vram[addr - 0x8000];
	}
	else if (addr >= 0xA000 && addr < 0xC000)
	{
		return  s_gb->gb_mem.sram[addr - 0xA000];
	}
	else if (addr >= 0xC000 && addr < 0xE000)
	{
		return  s_gb->gb_mem.ram[addr - 0xC000];
	}
	else if (addr >= 0xE000 && addr < 0xFE00)
	{
		return  s_gb->gb_mem.ram[addr - 0xE000];
	}
	else if (addr >= 0xFE00 && addr < 0xFF00)
	{
		return  s_gb->gb_mem.oam[addr - 0xFE00];
	}
	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		if (addr == 0xff00)
		{
			return (padState(s_gb));
		}
		if (addr == 0xff04)
		{
			return ((unsigned char)rand());
		}
		if (addr == 0xff0f)
		{
			return (s_gb->gb_interrupts.interFlag);
		}
		if (addr == 0xff41)
		{
			printf("reading lcd stat\n");
		}
		return  s_gb->gb_mem.io_ports[addr - 0xFF00];
	}
	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		return  s_gb->gb_mem.hram[addr - 0xFF80];
	}
	else if (addr == 0xffff) {
		return (s_gb->gb_interrupts.interEnable);
	}
	printf("read error : addr %x\n", addr);
	exit(-2);
}

void	mcbHandleBanking(unsigned short addr, unsigned char value, struct s_gb *s_gb)
{
	char	low5;

	low5 = value & 0x1f;
	//loRom bank change

	if (addr >= 0x2000 && addr < 0x4000) {
		if (s_gb->gb_rom->romheader.cartridgeType == 1) {
			MCB_romBanking &= 0xe0;
			MCB_romBanking |= low5;
			printf("Lo BANK change. value => %x\n", MCB_romBanking);
			}
	}
	//hiRom bank change
	else if (addr >= 0x4000 && addr < 0x6000)
	{
		if (romBankingFlag > 0)
		{
			MCB_romBanking &= 0x1f;
			value &= 0xe0;
			MCB_romBanking |= value;
			//printf("Hi BANK change. value => %x\n", MCB_romBanking);

		}
	}
	//change rom/ram bank
	else if (addr >= 0x6000 && addr < 0x8000)
	{
		romBankingFlag = ((value & 0x01) == 0) ? 1 : 0;

	}
	if (MCB_romBanking == 0)
		MCB_romBanking = 1;
}

int write8bit(unsigned short addr, unsigned char value, struct s_gb *s_gb)
{
	if (addr < 0x8000)
	{
		mcbHandleBanking(addr, value, s_gb);
		return 0;
	}
	else if (addr >= 0x8000 && addr < 0xA000)
	{

		s_gb->gb_mem.vram[addr - 0x8000] = value;
		return 0;
	}
	else if (addr >= 0xA000 && addr < 0xC000)
	{
		s_gb->gb_mem.sram[addr - 0xA000] = value;
		return 0;
	}
	else if (addr >= 0xC000 && addr < 0xE000)
	{
	
		s_gb->gb_mem.ram[addr - 0xC000] = value;
		return 0;
	}
	else if (addr >= 0xE000 && addr < 0xFE00)
	{
		s_gb->gb_mem.ram[addr - 0xE000] = value;
		return 0;
	}
	else if (addr >= 0xFE00 && addr < 0xFF00)
	{
		s_gb->gb_mem.oam[addr - 0xFE00] = value;
		return 0;
	}
	else if (addr >= 0xFF00 && addr < 0xFF80) {
		s_gb->gb_mem.io_ports[addr - 0xFF00] = value;
		if (addr == 0xff41)
		{
			printf("writing lcd stat %x\n", value);
		}
		ctrlIo(addr, (unsigned char *)s_gb->gb_mem.io_ports, s_gb);
		return 0;
	}
	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		s_gb->gb_mem.hram[addr - 0xFF80] = value;
		return 0;
	}
	else if (addr >= 0xffff) {
		s_gb->gb_interrupts.interEnable = value;
		return 0;
	}
	printf("write error addr = %x\n", addr);
	exit(-1);
}


unsigned char memoperation(unsigned short addr, signed char value, struct s_gb *s_gb)
{
	if (addr < 0x8000)
	{
//		s_rom->rom[addr] += value;
		printf("error memoperation on ROM space\n");
		exit(-2);
		return s_gb->gb_rom->rom[addr];
	}
	if (addr >= 0x8000 && addr < 0xA000)
	{
		s_gb->gb_mem.vram[addr - 0x8000] += value;
		 return s_gb->gb_mem.vram[addr - 0x8000];
	}
	if (addr >= 0xA000 && addr < 0xC000)
	{
		s_gb->gb_mem.sram[addr - 0xA000] += value;
		return s_gb->gb_mem.sram[addr - 0xA000];
	}
	else if (addr >= 0xC000 && addr < 0xE000)
	{
		s_gb->gb_mem.ram[addr - 0xC000] += value;
		return s_gb->gb_mem.ram[addr - 0xC000];
	}
	else if (addr >= 0xE000 && addr < 0xFE00)
	{
		s_gb->gb_mem.ram[addr - 0xE000] += value;
		return s_gb->gb_mem.ram[addr - 0xE000];
	}
	else if (addr >= 0xFE00 && addr < 0xFEFF)
	{
		s_gb->gb_mem.oam[addr - 0xFE00] += value;
		return s_gb->gb_mem.oam[addr - 0xFE00];
	}
	else if (addr >= 0xFF00 && addr < 0xFF80)
	{
		s_gb->gb_mem.io_ports[addr - 0xFF00] += value;
		return s_gb->gb_mem.io_ports[addr - 0xFF00];
	}
	else if (addr >= 0xFF80 && addr < 0xFFFF)
	{
		s_gb->gb_mem.hram[addr - 0xFF80] += value;
		return s_gb->gb_mem.hram[addr - 0xFF80];
	}
	else if (addr == 0xFFFF)
	{
		s_gb->gb_interrupts.interEnable = value;
		return s_gb->gb_interrupts.interEnable;
	}
	return 0;
}
