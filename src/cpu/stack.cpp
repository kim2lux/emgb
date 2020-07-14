#include "cpu.hpp"

void Z80Cpu::pop_bc0xc1()
{
    tickCount_ += 12;
    regs_.bc = mmu_.pop16(regs_.sp);
}
void Z80Cpu::pop_de0xd1()
{
    tickCount_ += 12;
    regs_.de = mmu_.pop16(regs_.sp);
}
void Z80Cpu::pop_hl0xe1()
{
    tickCount_ += 12;
    regs_.hl = mmu_.pop16(regs_.sp);
}

void Z80Cpu::pop_af0xf1()
{
    tickCount_ += 12;
    regs_.af = mmu_.pop16(regs_.sp);
    regs_.f &= 0xf0;
}

void Z80Cpu::push_af0xf5()
{
    tickCount_ += 16;
    mmu_.push16(regs_.sp, regs_.af);
}

void Z80Cpu::push_bc0xc5()
{
    tickCount_ += 16;
    mmu_.push16(regs_.sp, regs_.bc);
}

void Z80Cpu::push_hl0xe5()
{
    tickCount_ += 16;
    mmu_.push16(regs_.sp, regs_.hl);
}

void Z80Cpu::push_de0xd5()
{
    tickCount_ += 16;
    mmu_.push16(regs_.sp, regs_.de);
}
