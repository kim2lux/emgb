#include "GB.h"
#include "memory.hpp"

static uint8_t MCB_romBanking = 1;
static uint8_t romBankingFlag = 0;

void Memory::push16(uint16_t &stackPtr, uint16_t value)
{
    write16bitToAddr(stackPtr, value);
    stackPtr -= 2;
}


uint16_t Memory::pop16(uint16_t &stackPtr)
{
	uint16_t value;

	value = read16bit(stackPtr);
    stackPtr += 2;

	return (value);
}

void Memory::write16bitToAddr(uint16_t addr, uint16_t value)
{
    write8bit(addr, (uint8_t)value & 0x00ff);
    write8bit(addr + 1, (uint8_t)((value & 0xff00) >> 8));
}

uint16_t Memory::read16bit(uint16_t addr)
{
    uint16_t res = 0;

    res |= read8bit(addr);
    res |= read8bit(addr + 1) << 8;

    return res;
}

uint8_t Memory::read8bit(uint16_t addr)
{

    if (addr == 0xff44)
    {
        //return s_gb->gb_gpu.scanline;
    }
    else if (addr < 0x4000)
    {
        return (cart_.rom_[addr]);
    }
    else if (addr >= 0x4000 && addr < 0x8000)
    {
        //printf("MCB_romBanking value = %x\n", MCB_romBanking);
        return (cart_.rom_[(addr - 0x4000) + (MCB_romBanking * 0x4000)]);
    }
    else if (addr >= 0x8000 && addr < 0xA000)
    {
        return vram_[addr - 0x8000];
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        return sram_[addr - 0xA000];
    }
    else if (addr >= 0xC000 && addr < 0xE000)
    {
        if (addr == 0xc0b7)
            printf("read from ram 0xc0b7 %x\n", ram_[addr - 0xC000]);
        return ram_[addr - 0xC000];
    }
    else if (addr >= 0xE000 && addr < 0xFE00)
    {
        return ram_[addr - 0xE000];
    }
    else if (addr >= 0xFE00 && addr < 0xFF00)
    {
        return oam_[addr - 0xFE00];
    }
    else if (addr >= 0xFF00 && addr < 0xFF80)
    {
        if (addr == 0xff00)
        {
            //return (padState(s_gb));
        }
        if (addr == 0xff04)
        {
            return ((uint8_t)rand());
        }
        if (addr == 0xff0f)
        {
            //return (s_gb->gb_interrupts.interFlag);
        }
        if (addr == 0xff41)
        {
            printf("reading lcd stat\n");
        }
        return io_ports_[addr - 0xFF00];
    }
    else if (addr >= 0xFF80 && addr < 0xFFFF)
    {
        return hram_[addr - 0xFF80];
    }
    else if (addr == 0xffff)
    {
        //return (s_gb->gb_interrupts.interEnable);
    }
    printf("read error : addr %x\n", addr);
    exit(-2);
}

void Memory::mcbHandleBanking(uint16_t addr, uint8_t value)
{
    char low5;

    low5 = value & 0x1f;

    if (addr >= 0x2000 && addr < 0x4000)
    {
        if (cart_.getHeader().cartridgeType == 1)
        {
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

int Memory::write8bit(uint16_t addr, uint8_t value)
{
    if (addr < 0x8000)
    {
        mcbHandleBanking(addr, value);
        return 0;
    }
    else if (addr >= 0x8000 && addr < 0xA000)
    {
        vram_[addr - 0x8000] = value;
        return 0;
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        sram_[addr - 0xA000] = value;
        return 0;
    }
    else if (addr >= 0xC000 && addr < 0xE000)
    {
        ram_[addr - 0xC000] = value;
        return 0;
    }
    else if (addr >= 0xE000 && addr < 0xFE00)
    {
        ram_[addr - 0xE000] = value;
        return 0;
    }
    else if (addr >= 0xFE00 && addr < 0xFF00)
    {
        oam_[addr - 0xFE00] = value;
        return 0;
    }
    else if (addr >= 0xFF00 && addr < 0xFF80)
    {
        io_ports_[addr - 0xFF00] = value;
        if (addr == 0xff41)
        {
            printf("writing lcd stat %x\n", value);
        }
        //ctrlIo(addr, (uint8_t *)io_ports_);
        return 0;
    }
    else if (addr >= 0xFF80 && addr < 0xFFFF)
    {
        hram_[addr - 0xFF80] = value;
        return 0;
    }
    else if (addr >= 0xffff)
    {
        //s_gb->gb_interrupts.interEnable = value;
        return 0;
    }
    printf("write error addr = %x\n", addr);
    exit(-1);
}

uint8_t Memory::memoperation(uint16_t addr, int8_t value)
{
    if (addr < 0x8000)
    {
        printf("error memoperation on ROM space\n");
        exit(-2);
        return cart_.rom_[addr];
    }
    if (addr >= 0x8000 && addr < 0xA000)
    {
        vram_[addr - 0x8000] += value;
        return vram_[addr - 0x8000];
    }
    if (addr >= 0xA000 && addr < 0xC000)
    {
        sram_[addr - 0xA000] += value;
        return sram_[addr - 0xA000];
    }
    else if (addr >= 0xC000 && addr < 0xE000)
    {
        ram_[addr - 0xC000] += value;
        return ram_[addr - 0xC000];
    }
    else if (addr >= 0xE000 && addr < 0xFE00)
    {
        ram_[addr - 0xE000] += value;
        return ram_[addr - 0xE000];
    }
    else if (addr >= 0xFE00 && addr < 0xFEFF)
    {
        oam_[addr - 0xFE00] += value;
        return oam_[addr - 0xFE00];
    }
    else if (addr >= 0xFF00 && addr < 0xFF80)
    {
        io_ports_[addr - 0xFF00] += value;
        return io_ports_[addr - 0xFF00];
    }
    else if (addr >= 0xFF80 && addr < 0xFFFF)
    {
        hram_[addr - 0xFF80] += value;
        return hram_[addr - 0xFF80];
    }
    else if (addr == 0xFFFF)
    {
        //s_gb->gb_interrupts.interEnable = value;
        //return s_gb->gb_interrupts.interEnable;
    }
    return 0;
}

void Memory::memoryInit()
{
    memset(sram_, 0, 0x2000);
    memset(vram_, 0, 0x2000);
    memset(ram_, 0, 0x2000);
    memset(io_ports_, 0, 0x0080);

    write8bit(0xFF05, 0x00);
    write8bit(0xFF06, 0x00);
    write8bit(0xFF07, 0x08);
    write8bit(0xFF10, 0x80);
    write8bit(0xFF11, 0xBF);
    write8bit(0xFF12, 0xF3);
    write8bit(0xFF14, 0xBF);
    write8bit(0xFF16, 0x3F);
    write8bit(0xFF17, 0x00);
    write8bit(0xFF19, 0xBF);
    write8bit(0xFF1A, 0x7F);
    write8bit(0xFF1B, 0xFF);
    write8bit(0xFF1C, 0x9F);
    write8bit(0xFF1E, 0xBF);
    write8bit(0xFF20, 0xFF);
    write8bit(0xFF21, 0x00);
    write8bit(0xFF22, 0x00);
    write8bit(0xFF23, 0xBF);
    write8bit(0xFF24, 0x77);
    write8bit(0xFF25, 0xF3);
    write8bit(0xFF26, 0xF1);
    write8bit(0xFF40, 0x91);
    write8bit(0xFF42, 0x00);
    write8bit(0xFF43, 0x00);
    write8bit(0xFF45, 0x00);
    write8bit(0xFF47, 0xFC);
    write8bit(0xFF48, 0xFF);
    write8bit(0xFF49, 0xFF);
    write8bit(0xFF4A, 0x00);
    write8bit(0xFF4B, 0x00);
    write8bit(0xFFFF, 0x00);
}