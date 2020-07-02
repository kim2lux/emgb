#include "cpu.hpp"

void Z80Cpu::pop_bc()
{
    regs_.bc = mmu_.pop16(regs_.sp);
}
void Z80Cpu::pop_de()
{
    regs_.de = mmu_.pop16(regs_.sp);
}
void Z80Cpu::pop_hl()
{
    regs_.hl = mmu_.pop16(regs_.sp);
}
void Z80Cpu::pop_af()
{
    regs_.af = mmu_.pop16(regs_.sp);
}

void Z80Cpu::push_bc()
{
    mmu_.push16(regs_.sp, regs_.bc);
}

void Z80Cpu::push_hl()
{
    mmu_.push16(regs_.sp, regs_.hl);
}

void Z80Cpu::push_af()
{
    mmu_.push16(regs_.sp, regs_.af);
}

void Z80Cpu::push_de()
{
    mmu_.push16(regs_.sp, regs_.de);
}
