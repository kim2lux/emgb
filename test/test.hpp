#include "gtest/gtest.h"
#include "cpu.hpp"
#include "rom.hpp"
#include "memory"

class myTestFixture1 : public testing::Test
{
protected:
    Cartridge cart;
    Memory mem;
    Z80Cpu cpu;
    Joypad joypad;
    myTestFixture1() : mem(cart, joypad), cpu(mem)
    {
        cpu.tickCount_ = 0;
        cpu.regs_.bc = 0x0000;
        cpu.regs_.de = 0x1000;
        cpu.regs_.hl = 0x2000;
        cpu.regs_.sp = 0x3000;
    }

    virtual void SetUp() override
    {
        mem.write16bitToAddr(0xC000, 0xff00);
        mem.write16bitToAddr(0xC002, 0xabcd);
        mem.write16bitToAddr(0xC004, 0x4567);
        mem.write16bitToAddr(0xC006, 0xa45d);
    }
};
