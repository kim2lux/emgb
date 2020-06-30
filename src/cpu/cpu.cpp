#include "cpu.hpp"

uint8_t Z80Cpu::dec8(uint8_t &value)
{
    if (value & 0x0f)
        clear_half_carry();
    else
        set_half_carry();
    value -= 1;

    if (value)
        clear_zero();
    else
        set_zero();

    set_neg();
    return value;
}

void Z80Cpu::dec_a()
{
    dec8(regs_.a);
}

void Z80Cpu::nop()
{
}

void Z80Cpu::ld_bc_val_16() {

}


Z80Cpu::Z80Cpu(Memory &memory) : memory_(memory)
{
    opcodes_ = {
        {0x00, "NOP", std::bind(&Z80Cpu::nop, this), 1},
        {0x01, "ld_bc_val_16", std::bind(&Z80Cpu::ld_bc_val_16, this), 3},
    };
}


Memory &Z80Cpu::getMmu() {
    return memory_;
}
