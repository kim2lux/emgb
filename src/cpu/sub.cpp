#include "cpu.hpp"

void Z80Cpu::sub8bit(uint8_t &reg, uint8_t sub)
{
    tickCount_ += 4;
    set_neg_flag();

    if ((reg - sub) == 0x00)
    {
        set_zero_flag();
    }
    else
    {
        clear_zero_flag();
    }

    if ((reg & 0x0F) < (sub & 0x0F))
    {
        set_half_carry_flag();
    }
    else
    {
        clear_half_carry_flag();
    }

    if (reg < sub)
    {
        set_carry_flag();
    }
    else
    {
        clear_carry_flag();
    }

    reg = reg - sub;
}

void Z80Cpu::sub_a()
{
    sub8bit(regs_.a, regs_.a);
}

void Z80Cpu::sub_b()
{
    sub8bit(regs_.a, regs_.b);
}

void Z80Cpu::sub_c()
{
    sub8bit(regs_.a, regs_.c);
}

void Z80Cpu::sub_d()
{
    sub8bit(regs_.a, regs_.d);
}

void Z80Cpu::sub_e()
{
    sub8bit(regs_.a, regs_.e);
}

void Z80Cpu::sub_h()
{
    sub8bit(regs_.a, regs_.h);
}

void Z80Cpu::sub_l()
{
    sub8bit(regs_.a, regs_.l);
}

void Z80Cpu::sub_hl()
{
    tickCount_ += 4;
    sub8bit(regs_.a, mmu_.read8bit(regs_.hl));
}

void Z80Cpu::sub_a_8_d6() {
    tickCount_ += 4;
    sub8bit(regs_.a, mmu_.read8bit(regs_.pc));
}