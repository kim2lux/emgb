#include "cpu.hpp"

void Z80Cpu::sbc8bit(uint8_t &reg, uint8_t sub) {
	uint8_t carry = isFlagSet(Flag::CARRY_FLAG) ? 1 : 0;
	int32_t result = reg - sub - carry;

	clear_flags();
	set_neg_flag();
	if (static_cast<uint8_t>(result) == 0x00) {
		set_zero_flag();
	}

	if (result < 0) {
		set_carry_flag();
	}

	if (((reg & 0x0F) - (sub & 0x0F) - carry) < 0) {
        set_half_carry_flag();
	}

	reg = static_cast<uint8_t>(result);
}

void Z80Cpu::sbc_a()
{
    sbc8bit(regs_.a, regs_.a);
}

void Z80Cpu::sbc_b()
{
    sbc8bit(regs_.a, regs_.b);
}

void Z80Cpu::sbc_c()
{
    sbc8bit(regs_.a, regs_.c);
}

void Z80Cpu::sbc_d()
{
    sbc8bit(regs_.a, regs_.d);
}

void Z80Cpu::sbc_e()
{
    sbc8bit(regs_.a, regs_.e);
}

void Z80Cpu::sbc_h()
{
    sbc8bit(regs_.a, regs_.h);
}

void Z80Cpu::sbc_l()
{
    sbc8bit(regs_.a, regs_.l);
}

void Z80Cpu::sbc_hl()
{
    tickCount_ += 4;
    sbc8bit(regs_.a, mmu_.read8bit(regs_.hl));
}
