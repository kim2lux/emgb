#include "cpu.hpp"

void Z80Cpu::dec8BitRegister(uint8_t &reg) {
    tickCount_ += 4;
    reg -= 1;
    set_neg_flag();
    if (reg == 0) {
        set_zero_flag();
    } else {
        clear_zero_flag();
    }

    if ((reg & 0x0f) == 0x0f) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }
}

void Z80Cpu::dec_b_0x05() {
    dec8BitRegister(regs_.b);
}

void Z80Cpu::dec_d_0x15() {
    dec8BitRegister(regs_.d);
}

void Z80Cpu::dec_h_0x25() {
    dec8BitRegister(regs_.h);
}

void Z80Cpu::dec_hl_addr_0x35() {
    tickCount_ += 12;
    mmu_.memoperation(regs_.hl, -1);
}
