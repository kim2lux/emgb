#include "cpu.hpp"

/*
#define ZERO_FLAG (s_gb->gb_register.f & 0x80)
#define CARRY_FLAG (s_gb->gb_register.f & 0x10)
#define NEG_FLAG (s_gb->gb_register.f & 0x40)
#define HALFC_FLAG (s_gb->gb_register.f & 0x20)
*/

bool Z80Cpu::isFlagSet(Flag f) {
    switch (f) {
        case Flag::CARRY_FLAG:
            return (regs_.f & 0x10);
        case Flag::ZERO_FLAG:
            return (regs_.f & 0x80);
        case Flag::NEG_FLAG:
            return (regs_.f & 0x40);
        case Flag::HALFC_FLAG:
            return (regs_.f & 0x20);
    }
}

void Z80Cpu::scf_0x36() {
    tickCount_ += 4;
    clear_neg_flag();
    clear_half_carry_flag();
    set_carry_flag();
}


void Z80Cpu::initRegister()
{
    regs_.af = 0x01;
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
        {0x05, "dec_b_0x05", std::bind(&Z80Cpu::dec_b_0x05, this), 1},
        {0x06, "ld_8_to_b_0x06", std::bind(&Z80Cpu::ld_8_to_b_0x06, this), 2},
        {0x07, "rlca_0x07", std::bind(&Z80Cpu::rlca_0x07, this), 1},
        {0x11, "ld_16_to_de_0x11", std::bind(&Z80Cpu::ld_16_to_de_0x11, this), 3},
        {0x12, "ld_a_to_de_addr_0x12", std::bind(&Z80Cpu::ld_a_to_de_addr_0x12, this), 1},
        {0x13, "inc_bc_0x03", std::bind(&Z80Cpu::inc_de_0x13, this), 1},
        {0x14, "inc_d_0x14", std::bind(&Z80Cpu::inc_d_0x14, this), 1},
        {0x15, "dec_d_0x15", std::bind(&Z80Cpu::dec_d_0x15, this), 1},
        {0x16, "ld_8_to_d_0x16", std::bind(&Z80Cpu::ld_8_to_d_0x16, this), 2},
        {0x17, "rla_0x08", std::bind(&Z80Cpu::rla_0x17, this), 1},
        {0x21, "ld_16_to_hl_0x21", std::bind(&Z80Cpu::ld_16_to_hl_0x21, this), 3},
        {0x22, "ld_a_to_hl_addr_0x22", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x22, this), 1},
        {0x23, "inc_bc_0x23", std::bind(&Z80Cpu::inc_hl_0x23, this), 1},
        {0x24, "inc_h_0x14", std::bind(&Z80Cpu::inc_h_0x24, this), 1},
        {0x25, "dec_h_0x25", std::bind(&Z80Cpu::dec_h_0x25, this), 1},
        {0x26, "ld_8_to_h_0x26", std::bind(&Z80Cpu::ld_8_to_h_0x26, this), 2},
        {0x27, "daa_0x27", std::bind(&Z80Cpu::daa_0x27, this), 2},
        {0x31, "ld_16_to_sp_0x31", std::bind(&Z80Cpu::ld_16_to_sp_0x31, this), 3},
        {0x32, "ld_a_to_hl_addr_0x32", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x32, this), 1},
        {0x33, "inc_bc_0x33", std::bind(&Z80Cpu::inc_sp_0x33, this), 1},
        {0x34, "inc_hl_addr_0x34", std::bind(&Z80Cpu::inc_hl_addr_0x34, this), 1},
        {0x35, "dec_hl_addr_0x35", std::bind(&Z80Cpu::dec_hl_addr_0x35, this), 1},
        {0x36, "ld_8_to_hl_addr_0x36", std::bind(&Z80Cpu::ld_8_to_hl_addr_0x36, this), 2},
        {0x37, "scf_0x36", std::bind(&Z80Cpu::scf_0x36, this), 1},
    };
}

Memory &Z80Cpu::getMmu()
{
    return mmu_;
}
