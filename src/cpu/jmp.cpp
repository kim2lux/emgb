#include "cpu.hpp"

void Z80Cpu::jmp_8_0x18()
{
    tickCount_ += 12;
    regs_.pc += (int8_t)mmu_.read8bit(regs_.pc) + 1;
}

void Z80Cpu::jmp_8_if_not_zero_0x20()
{
    uint16_t tick = 8;
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc);
    }
    tickCount_ += tick;
    regs_.pc += 1;
}

void Z80Cpu::jmp_8_if_not_carry_0x30()
{
    uint16_t tick = 8;
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc);
    }
    tickCount_ += tick;
    regs_.pc += 1;
}

void Z80Cpu::jmp_8_if_zero_0x28()
{
    uint16_t tick = 8;
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc);
    }
    tickCount_ += tick;
    regs_.pc += 1;
}

void Z80Cpu::jmp_8_if_carry_0x38()
{
    uint16_t tick = 8;
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc);
    }
    tickCount_ += tick;
    regs_.pc += 1;
}

void Z80Cpu::ret_nz()
{
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_nc()
{
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_z()
{
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_c()
{
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}


void Z80Cpu::jmp_nz()
{
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
    }
    else
    {
        regs_.pc += 2;
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_nc()
{
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
    }
    else
    {
        regs_.pc += 2;
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_z()
{
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
    }
    else
    {
        regs_.pc += 2;
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_c()
{
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
    }
    else
    {
        regs_.pc += 2;
        tickCount_ += 12;
    }
}