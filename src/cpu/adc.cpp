#include "cpu.hpp"


void Z80Cpu::adc8Bit(uint8_t &reg, uint8_t add) {
    uint8_t carry = 0x00;
    tickCount_ += 4;

	if (isFlagSet(Flag::CARRY_FLAG)) {
		carry = 0x01;
	}

	uint16_t result = reg + carry + add;

	clear_neg_flag();
	if ((result & 0xFF) == 0x00) {
		set_zero_flag();
	} else {
        clear_zero_flag();
	}

	if ((carry + (add & 0x0F) + (reg & 0x0F)) > 0x0F) {
		set_half_carry_flag();
	} else {
		clear_half_carry_flag();
	}

	if (result > 0xFF) {
		set_carry_flag();
	} else {
		clear_carry_flag();
	}

	reg = result & 0xFF;
}

void Z80Cpu::adc_b() {
    adc8Bit(regs_.a, regs_.b);
}

void Z80Cpu::adc_c() {
    adc8Bit(regs_.a, regs_.c);
}
void Z80Cpu::adc_d() {
    adc8Bit(regs_.a, regs_.d);
}
void Z80Cpu::adc_e() {
    adc8Bit(regs_.a, regs_.e);
}

void Z80Cpu::adc_h() {
    adc8Bit(regs_.a, regs_.h);
}

void Z80Cpu::adc_l() {
    adc8Bit(regs_.a, regs_.l);
}

void Z80Cpu::adc_hl() {
    tickCount_ += 4;
    adc8Bit(regs_.a, mmu_.read8bit(regs_.hl));
}

void Z80Cpu::adc_a() {
    adc8Bit(regs_.a, regs_.a);
}
