#include "memory.hpp"
#include <iostream>
#include "utils.h"
static constexpr uint16_t OAM_REG_ADDR = 0xFF46;
static constexpr uint16_t OAM_MEM_START = 0xFE00;

void Memory::push16(uint16_t &stackPtr, uint16_t value)
{
    stackPtr -= 2;
    write16bitToAddr(stackPtr, value);
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
    static uint8_t scanline = 0;
    if (addr < 0x8000) // cartridge region
    {
        return (cart_.rom_[addr]);
    }
    if (addr == 0xff44) {
        return scanline++;
    }
    else if (addr >= 0xFF00 && addr < 0xFF80) // IO register region
    {
        if (addr == joypad_state_addr)
        {
            return (joypad_.padState());
        }
    }
    return (mmu_.raw[addr]);
}

int Memory::write8bit(uint16_t addr, uint8_t value)
{
    mmu_.raw[addr] = value;
    if (addr >= 0xA000 && addr < 0xC000)
    {
        cart_.rom_[addr] = value;
    }
    if (addr == joypad_state_addr)
    {
        joypad_.key_ = value;
    }
    if (addr == OAM_REG_ADDR) {
        dmaTransfer(value);
    }
    if (addr == serial_data_addr)
    {
        //std::cout << (char)value << std::endl;
    }
    return (0);
}

void Memory::dmaTransfer(uint8_t value) {
    uint16_t srcAddr = value * 100;
    for (uint16_t idx = 0; idx < 0x9f; ++idx) {
        write8bit(OAM_MEM_START + idx, read8bit(srcAddr + idx));
    }
}

uint8_t Memory::memoperation(uint16_t addr, int8_t value)
{
    mmu_.raw[addr] += value;
    return 0;
}

void Memory::memoryInit()
{
    memset(mmu_.raw, 0, 0xffff);
    write8bit(0xFF05, 0x00); //tima
    write8bit(0xFF06, 0x00); //tma
    write8bit(0xFF07, 0x00); //tac
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
    write8bit(0xFF40, 0x91); //LCDC
    write8bit(0xFF42, 0x00); //SCY
    write8bit(0xFF43, 0x00); //SCX
    write8bit(0xFF45, 0x00); //LYC
    write8bit(0xFF47, 0xFC); //BGP
    write8bit(0xFF48, 0xFF);
    write8bit(0xFF49, 0xFF);
    write8bit(0xFF4A, 0x00);
    write8bit(0xFF4B, 0x00);
    write8bit(0xFFFF, 0x00);
}