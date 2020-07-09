#include "cpu.hpp"

void Z80Cpu::dec8BitRegister(uint8_t &reg)
{
    tickCount_ += 4;
    reg -= 1;
    set_neg_flag();
    if (reg == 0)
    {
        set_zero_flag();
    }
    else
    {
        clear_zero_flag();
    }

    if ((reg & 0x0f) == 0x0f)
    {
        set_half_carry_flag();
    }
    else
    {
        clear_half_carry_flag();
    }
}

void Z80Cpu::dec_b_0x05()
{
    dec8BitRegister(regs_.b);
}

void Z80Cpu::dec_d_0x15()
{
    dec8BitRegister(regs_.d);
}

void Z80Cpu::dec_h_0x25()
{
    dec8BitRegister(regs_.h);
}

void Z80Cpu::dec_hl_addr_0x35()
{
    uint8_t value = getMemory().read8bit(regs_.hl);
    dec8BitRegister(value);
    getMemory().write8bit(regs_.hl, value);
    tickCount_ += 8;
}

void Z80Cpu::dec_bc_0x0b()
{
    tickCount_ += 8;
    regs_.bc -= 1;
}
void Z80Cpu::dec_de_0x1b()
{
    tickCount_ += 8;
    regs_.de -= 1;
}
void Z80Cpu::dec_hl_0x2b()
{
    tickCount_ += 8;
    regs_.hl -= 1;
}
void Z80Cpu::dec_sp_0x3b()
{
    tickCount_ += 8;
    regs_.sp -= 1;
}

void Z80Cpu::dec_c_0x0d()
{
    dec8BitRegister(regs_.c);
}
void Z80Cpu::dec_e_0x1d()
{
    dec8BitRegister(regs_.e);
}
void Z80Cpu::dec_l_0x2d()
{
    dec8BitRegister(regs_.l);
}
void Z80Cpu::dec_a_0x3d()
{
    dec8BitRegister(regs_.a);
}
