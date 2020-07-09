#include "cpu.hpp"

void Z80Cpu::jmp_8_0x18()
{
    tickCount_ += 12;
    regs_.pc += static_cast<char>(mmu_.read8bit(regs_.pc)) + 1;
    fjmp_ = true;
}

void Z80Cpu::jmp_8_if_not_zero_0x20()
{
    uint16_t tick = 8;
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tick = 12;
        int8_t val = static_cast<char>(mmu_.read8bit(regs_.pc));
        regs_.pc += val + 1;
        fjmp_ = true;
    }
    else {
        tick = 8;
    }
    tickCount_ += tick;
}

void Z80Cpu::jmp_8_if_not_carry_0x30()
{
    uint16_t tick = 8;
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tick = 12;
        regs_.pc += static_cast<char>(mmu_.read8bit(regs_.pc)) + 1;
        fjmp_ = true;
    }
    tickCount_ += tick;
}

void Z80Cpu::jmp_8_if_zero_0x28()
{
    uint16_t tick = 8;
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc) + 1;
        fjmp_ = true;
    }
    tickCount_ += tick;
}

void Z80Cpu::jmp_8_if_carry_0x38()
{
    uint16_t tick = 8;
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tick = 12;
        regs_.pc += (int8_t)mmu_.read8bit(regs_.pc) + 1;
        fjmp_ = true;
    }
    tickCount_ += tick;
}

void Z80Cpu::ret_0xc9()
{
    tickCount_ += 16;
    regs_.pc = mmu_.pop16(regs_.sp);
}

void Z80Cpu::reti_0xd9() {
    tickCount_ += 16;
    regs_.pc = mmu_.pop16(regs_.sp);
    interrupt_.masterInterrupt_ = true;
}

void Z80Cpu::ret_nz0xc0()
{
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_nc0xd0()
{
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_z0xc8()
{
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::ret_c0xd8()
{
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 20;
        regs_.pc = mmu_.pop16(regs_.sp);
    }
    tickCount_ += 8;
}

void Z80Cpu::jmp_nz0xc2()
{
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
        fjmp_ = true;
    }
    else
    {
        regs_.pc += 2;
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_nc0xd2()
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

void Z80Cpu::jmp_hl_0xe9() {
    tickCount_ += 4;
    regs_.pc = regs_.hl;
}

void Z80Cpu::jmp_z0xca()
{
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
        fjmp_ = true;
    }
    else
    {
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_c0xda()
{
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 16;
        regs_.pc = mmu_.read16bit(regs_.pc);
        fjmp_ = true;
    }
    else
    {
        fjmp_ = true;
        tickCount_ += 12;
    }
}

void Z80Cpu::jmp_160xc3()
{
    tickCount_ += 16;
    regs_.pc = mmu_.read16bit(regs_.pc);
    fjmp_ = true;
}

void Z80Cpu::call_nz_16_0xc4()
{
    if (!isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 24;
        uint16_t value = mmu_.read16bit(regs_.pc);
        mmu_.push16(regs_.sp, regs_.pc + 2);
        regs_.pc = value;
        fjmp_ = true;
    }
    else
    {
        tickCount_ += 12;
    }
}

void Z80Cpu::call_nc_16_0xd4()
{
    if (!isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 24;
        uint16_t value = mmu_.read16bit(regs_.pc);
        mmu_.push16(regs_.sp, regs_.pc + 2);
        regs_.pc = value;
        fjmp_ = true;
    }
    else
    {
        tickCount_ += 12;
    }
}

void Z80Cpu::call_z_16_0xcc()
{
    if (isFlagSet(Flag::ZERO_FLAG))
    {
        tickCount_ += 24;
        uint16_t value = mmu_.read16bit(regs_.pc);
        mmu_.push16(regs_.sp, regs_.pc + 2);
        regs_.pc = value;
        fjmp_ = true;
    }
    else
    {
        tickCount_ += 12;
    }
}

void Z80Cpu::call_c_16_0xdc()
{
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        tickCount_ += 24;
        uint16_t value = mmu_.read16bit(regs_.pc);
        mmu_.push16(regs_.sp, regs_.pc + 2);
        regs_.pc = value;
        fjmp_ = true;
    }
    else
    {
        tickCount_ += 12;
    }
}

void Z80Cpu::call_16_0xcd()
{
    tickCount_ += 24;
    uint16_t value = mmu_.read16bit(regs_.pc);
    mmu_.push16(regs_.sp, regs_.pc + 2);
    regs_.pc = value;
    fjmp_ = true;
}
