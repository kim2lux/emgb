#include "cpu.hpp"

void Z80Cpu::add_hl_16(uint16_t value) {
    tickCount_ += 8;
    clear_neg_flag();

    if ((regs_.hl & 0x0fff) + (value & 0x0fff) > 0x0fff ) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }

    if ((regs_.hl + value) > 0xffff ) {
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
    regs_.hl += value;
}

void Z80Cpu::add_hl_bc_0x09()
{
    add_hl_16(regs_.bc);
}
void Z80Cpu::add_hl_de_0x19()
{
    add_hl_16(regs_.de);
}
void Z80Cpu::add_hl_hl_0x29()
{
    add_hl_16(regs_.hl);
}
void Z80Cpu::add_hl_sp_0x39()
{
    add_hl_16(regs_.sp);
}

void Z80Cpu::add8bit(uint8_t &reg, uint8_t add) {
    tickCount_ += 4;
    if ((reg + add) & 0xff) {
        clear_zero_flag();
    } else {
        set_zero_flag();
    }
    clear_neg_flag();
    if (((reg & 0x0f) + (add & 0x0f)) > 0x0f) {
        set_half_carry_flag();
    } else {
        clear_half_carry_flag();
    }


    if ((reg + add) > 0xff) {
        set_carry_flag();
    } else {
        clear_carry_flag();
    }
    reg += add;
}

void Z80Cpu::add_b() {
    add8bit(regs_.a, regs_.b);
}

void Z80Cpu::add_a() {
    add8bit(regs_.a, regs_.a);
}

void Z80Cpu::add_c() {
    add8bit(regs_.a, regs_.c);
}

void Z80Cpu::add_d() {
    add8bit(regs_.a, regs_.d);
}

void Z80Cpu::add_e() {
    add8bit(regs_.a, regs_.e);
}

void Z80Cpu::add_h() {
    add8bit(regs_.a, regs_.h);
}

void Z80Cpu::add_l() {
    add8bit(regs_.a, regs_.l);
}

void Z80Cpu::add_hl() {
    tickCount_ += 4;
    add8bit(regs_.a, mmu_.read8bit(regs_.hl));
}
