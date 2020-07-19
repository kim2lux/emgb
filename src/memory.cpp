#include "memory.hpp"
#include <iostream>
#include "utils.h"
#include "gpu.hpp"

static constexpr uint16_t OAM_REG_ADDR = 0xFF46;
static constexpr uint16_t OAM_MEM_START = 0xFE00;

void Memory::setGpu(std::shared_ptr<Gpu> gpu) {
    gpu_ = gpu;
}

void Memory::setCpu(std::shared_ptr<Z80Cpu> cpu) {
    cpu_ = cpu;
}

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
    if (addr < 0x8000)
    {
        return (cart_.read8bit(addr));
    }
    else if (addr >= 0xA000 && addr < 0xC000) {
        return (cart_.read8bit(addr));
    }
    else if (addr == joypad_state_addr)
    {
        return (joypad_.padState());
    }
    else if (addr == 0xff44) {
        return gpu_.get()->getScanline();
    }
    return (mmu_.raw[addr]);
}

void Memory::write8bit(uint16_t addr, uint8_t value)
{
    if (addr < 0x8000) {
        cart_.write8bit(addr, value);
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        cart_.write8bit(addr, value);
    }
    else if (addr >= 0xC000 && addr <= 0xDFFF) {
        mmu_.raw[addr] = value;
		if (addr <= 0xDDFF) {
			mmu_.raw[addr + 0x2000] = value;
		}
    }
    else if ( addr > 0xE000 && addr <= 0xFDFF ) {
        mmu_.raw[addr] = value;
        mmu_.raw[addr - 0x2000] = value;
    }
    else if (addr == 0xff04) {
        mmu_.raw[addr] = value;
        cpu_.get()->timer_.divRegister_ = 0;
    }
    else if (addr == Timer::tac_addr && cpu_.get() != nullptr) {
        switch(value & 0x03) {
            case 0: cpu_.get()->timer_.clockSpeed_ = 1024; break;
            case 1: cpu_.get()->timer_.clockSpeed_ = 16; break;
            case 2: cpu_.get()->timer_.clockSpeed_ = 64; break;
            case 3: cpu_.get()->timer_.clockSpeed_ = 256; break;
        }
        mmu_.raw[addr] = value | 0xf8;
    }
    else if (addr == LCD_DISPLAY_CTRL) {

        mmu_.raw[addr] = value;
        if (gpu_.get() != nullptr) {
            if (!isBitSet(value, 7) && gpu_.get()->isLcdEnable())
            {
                gpu_.get()->disableLcd();
            }
            else if (isBitSet(value, 7) && !gpu_.get()->isLcdEnable())
            {
                gpu_.get()->getLcdEnable() = true;
            }
        }
    }
    else if (addr == LCDC_STATUS_ADDR) {
		// Don't allow the override of values in bits [0,2]
		uint8_t lcdcStatus =  mmu_.raw[addr];
        //bit 3-6
		uint8_t relevantData = value & 0x78;
        //relevantData = updated bit 3-6  + (lcdcStatus & 0x07) = old bit 0 - 2 as previous lcdcStatus
		uint8_t currentLCDCStatus = relevantData | (lcdcStatus & 0x07);
		mmu_.raw[addr] = currentLCDCStatus;

        bool triggerLcdInterrupt = false;
        if (gpu_.get() != nullptr) {
            if (gpu_.get()->isLcdEnable())
            {
                GpuMode currentMode = gpu_.get()->getGpuMode();
                switch (currentMode)
                {
                case GpuMode::H_BLANK:
                    triggerLcdInterrupt = isBitSet(currentLCDCStatus, 3);
                    break;
                case GpuMode::V_BLANK:
                    triggerLcdInterrupt = isBitSet(currentLCDCStatus, 4);
                    break;
                case GpuMode::OAM:
                    triggerLcdInterrupt = isBitSet(currentLCDCStatus, 5);
                    break;
                case GpuMode::LCD_TX:
                    break;
                }
            }
            if (triggerLcdInterrupt == true)
            {
                uint8_t interruptRequest_ = read8bit(IF_REGISTER);
                setBit(interruptRequest_, 1);
                mmu_.raw[IF_REGISTER] = interruptRequest_;
            }
        }
    }
    else if (addr == joypad_state_addr)
    {
        mmu_.raw[addr] = value;
        joypad_.key_ = value;
    }
    else if (addr == OAM_REG_ADDR) {
        mmu_.raw[addr] = value;
        dmaTransfer(value);
    }
    else if (addr == serial_data_addr)
    {
        mmu_.raw[addr] = value;
        std::cout << "serial: " << (char)value << std::endl;
    }
    else
    {
        mmu_.raw[addr] = value;
    }
}

void Memory::dmaTransfer(uint8_t value) {
    uint16_t srcaddr = value << 8;
    for (uint16_t idx = 0; idx < 0xa0; ++idx) {
        write8bit(OAM_MEM_START + idx, read8bit(srcaddr + idx));
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