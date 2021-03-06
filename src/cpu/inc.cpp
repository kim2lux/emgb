#include "cpu.hpp"

void Z80Cpu::inc_bc_0x03()
{
    tickCount_ += 8;
    regs_.bc += 1;
}

void Z80Cpu::inc_de_0x13()
{
    tickCount_ += 8;
    regs_.de += 1;
}

void Z80Cpu::inc_hl_0x23()
{
    tickCount_ += 8;
    regs_.hl += 1;
}

void Z80Cpu::inc_sp_0x33()
{
    tickCount_ += 8;
    regs_.sp += 1;
}

void Z80Cpu::inc8BitRegister(uint8_t &reg)
{
    tickCount_ += 4;
    reg += 1;
    clear_neg_flag();
    if (reg == 0x00)
    {
        set_zero_flag();
    }
    else
    {
        clear_zero_flag();
    }

    if (reg & 0x0f)
    {
        clear_half_carry_flag();
    }
    else
    {
        set_half_carry_flag();
    }
}

void Z80Cpu::inc_b_0x04()
{
    inc8BitRegister(regs_.b);
}

void Z80Cpu::inc_d_0x14()
{
    inc8BitRegister(regs_.d);
}

void Z80Cpu::inc_h_0x24()
{
    inc8BitRegister(regs_.h);
}

void Z80Cpu::inc_hl_addr_0x34()
{
    tickCount_ += 8;
    uint8_t value = getMemory().read8bit(regs_.hl);
    inc8BitRegister(value);
    getMemory().write8bit(regs_.hl, value);
}

void Z80Cpu::inc_c_0x0c()
{
    inc8BitRegister(regs_.c);
}

void Z80Cpu::inc_e_0x1c()
{
    inc8BitRegister(regs_.e);
}
void Z80Cpu::inc_l_0x2c()
{
    inc8BitRegister(regs_.l);
}
void Z80Cpu::inc_a_0x3c()
{
    inc8BitRegister(regs_.a);
}