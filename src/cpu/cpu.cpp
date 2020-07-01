#include "cpu.hpp"

uint8_t Z80Cpu::dec8(uint8_t &value)
{
    if (value & 0x0f)
        clear_half_carry_flag();
    else
        set_half_carry_flag();
    value -= 1;

    if (value)
        clear_zero_flag();
    else
        set_zero_flag();

    set_neg_flag();
    return value;
}

void Z80Cpu::dec_a()
{
    dec8(regs_.a);
}

void Z80Cpu::nop_0x00()
{
}

void Z80Cpu::initRegister() {
    regs_.af =0x01;
    regs_.f = 0xb0;
    regs_.bc = 0x0013;
    regs_.de = 0x00d8;
    regs_.hl = 0x014d;
    regs_.sp = 0xfffe;
}

Z80Cpu::Z80Cpu(Memory &memory) : mmu_(memory)
{
    initRegister();
    opcodes_ = {
        {0x00, "NOP", std::bind(&Z80Cpu::nop_0x00, this), 1},
        {0x01, "ld_16_to_bc_0x01", std::bind(&Z80Cpu::ld_16_to_bc_0x01, this), 3},
        {0x02, "ld_a_to_bc_addr_0x02", std::bind(&Z80Cpu::ld_a_to_bc_addr_0x02, this), 1},
        {0x03, "inc_bc_0x03", std::bind(&Z80Cpu::inc_bc_0x03, this), 1},
        {0x04, "inc_b_0x04", std::bind(&Z80Cpu::inc_b_0x04, this), 1},
        {0x11, "ld_16_to_de_0x11", std::bind(&Z80Cpu::ld_16_to_de_0x11, this), 3},
        {0x12, "ld_a_to_de_addr_0x12", std::bind(&Z80Cpu::ld_a_to_de_addr_0x12, this), 1},
        {0x13, "inc_bc_0x03", std::bind(&Z80Cpu::inc_de_0x13, this), 1},
        {0x14, "inc_d_0x14", std::bind(&Z80Cpu::inc_d_0x14, this), 1},
        {0x21, "ld_16_to_hl_0x21", std::bind(&Z80Cpu::ld_16_to_hl_0x21, this), 3},
        {0x22, "ld_a_to_hl_addr_0x22", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x22, this), 1},
        {0x23, "inc_bc_0x23", std::bind(&Z80Cpu::inc_hl_0x23, this), 1},
        {0x24, "inc_h_0x14", std::bind(&Z80Cpu::inc_h_0x24, this), 1},
        {0x31, "ld_16_to_sp_0x31", std::bind(&Z80Cpu::ld_16_to_sp_0x31, this), 3},
        {0x32, "ld_a_to_hl_addr_0x32", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x32, this), 1},
        {0x33, "inc_bc_0x33", std::bind(&Z80Cpu::inc_sp_0x33, this), 1},
        {0x34, "inc_hl_addr_0x34", std::bind(&Z80Cpu::inc_hl_addr_0x34, this), 1},
    };
}


Memory &Z80Cpu::getMmu() {
    return mmu_;
}
