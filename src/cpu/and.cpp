#include "cpu.hpp"

void Z80Cpu::cp8bit(uint8_t & reg, uint8_t value) {
	clear_flags();

    if (reg == value) {
        set_zero_flag();
    }
    if ((reg & 0x0f) < (value & 0x0f)) {
        set_half_carry_flag();
    }
	if (value > reg) {
		set_carry_flag();
	}
}

void Z80Cpu::cp_b() {
    cp8bit(regs_.a, regs_.b);
}

void Z80Cpu::cp_a() {
    cp8bit(regs_.a, regs_.a);
}

void Z80Cpu::cp_c() {
    cp8bit(regs_.a, regs_.c);
}

void Z80Cpu::cp_d() {
    cp8bit(regs_.a, regs_.d);
}

void Z80Cpu::cp_e() {
    cp8bit(regs_.a, regs_.e);
}

void Z80Cpu::cp_h() {
    cp8bit(regs_.a, regs_.h);
}

void Z80Cpu::cp_l() {
    cp8bit(regs_.a, regs_.l);
}

void Z80Cpu::cp_hl() {
    tickCount_ += 4;
    cp8bit(regs_.a, mmu_.read8bit(regs_.hl));
}

void Z80Cpu::or8bit(uint8_t & reg, uint8_t value) {
	reg = reg | value;

	clear_flags();
	if (reg == 0x00) {
		set_zero_flag();
	}
}

void Z80Cpu::or_b() {
    or8bit(regs_.a, regs_.b);
}

void Z80Cpu::or_a() {
    or8bit(regs_.a, regs_.a);
}

void Z80Cpu::or_c() {
    or8bit(regs_.a, regs_.c);
}

void Z80Cpu::or_d() {
    or8bit(regs_.a, regs_.d);
}

void Z80Cpu::or_e() {
    or8bit(regs_.a, regs_.e);
}

void Z80Cpu::or_h() {
    or8bit(regs_.a, regs_.h);
}

void Z80Cpu::or_l() {
    or8bit(regs_.a, regs_.l);
}

void Z80Cpu::or_hl() {
    tickCount_ += 4;
    or8bit(regs_.a, mmu_.read8bit(regs_.hl));
}

void Z80Cpu::xor8bit(uint8_t & reg, uint8_t value) {
	reg = reg ^ value;

	clear_flags();
	if (reg == 0x00) {
		set_zero_flag();
	}
}

void Z80Cpu::xor_b() {
    xor8bit(regs_.a, regs_.b);
}

void Z80Cpu::xor_a() {
    xor8bit(regs_.a, regs_.a);
}

void Z80Cpu::xor_c() {
    xor8bit(regs_.a, regs_.c);
}

void Z80Cpu::xor_d() {
    xor8bit(regs_.a, regs_.d);
}

void Z80Cpu::xor_e() {
    xor8bit(regs_.a, regs_.e);
}

void Z80Cpu::xor_h() {
    xor8bit(regs_.a, regs_.h);
}

void Z80Cpu::xor_l() {
    xor8bit(regs_.a, regs_.l);
}

void Z80Cpu::xor_hl() {
    tickCount_ += 4;
    xor8bit(regs_.a, mmu_.read8bit(regs_.hl));
}

void Z80Cpu::and8bit(uint8_t & reg, uint8_t value) {
	reg = reg & value;

	clear_flags();
	set_half_carry_flag();
	if (reg == 0x00) {
		set_zero_flag();
	}
}


void Z80Cpu::and_b() {
    and8bit(regs_.a, regs_.b);
}

void Z80Cpu::and_a() {
    and8bit(regs_.a, regs_.a);
}

void Z80Cpu::and_c() {
    and8bit(regs_.a, regs_.c);
}

void Z80Cpu::and_d() {
    and8bit(regs_.a, regs_.d);
}

void Z80Cpu::and_e() {
    and8bit(regs_.a, regs_.e);
}

void Z80Cpu::and_h() {
    and8bit(regs_.a, regs_.h);
}

void Z80Cpu::and_l() {
    and8bit(regs_.a, regs_.l);
}

void Z80Cpu::and_hl() {
    tickCount_ += 4;
    and8bit(regs_.a, mmu_.read8bit(regs_.hl));
}
