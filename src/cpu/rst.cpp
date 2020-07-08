#include "cpu.hpp"

void Z80Cpu::rst(const uint16_t addr)
{
    tickCount_ += 16;
    mmu_.push16(regs_.sp, regs_.pc);
    regs_.pc = addr;
}

void Z80Cpu::rst_0_0xc7()
{
    rst(0);
}
void Z80Cpu::rst_10_0xd7()
{
    rst(0x10);
}
void Z80Cpu::rst_20_0xe7()
{
    rst(0x20);
}
void Z80Cpu::rst_30_0xf7()
{
    rst(0x30);
}
void Z80Cpu::rst_8_0xcf()
{
    rst(0x8);
}
void Z80Cpu::rst_18_0xdf()
{
    rst(0x18);
}
void Z80Cpu::rst_28_0xef()
{
    rst(0x28);
}
void Z80Cpu::rst_38_0xff()
{
    rst(0x38);
}