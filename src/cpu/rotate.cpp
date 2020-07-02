#include "cpu.hpp"

void Z80Cpu::nop_0x00()
{
	tickCount_ += 4;
}

void Z80Cpu::daa_0x27() 
{
    tickCount_ += 4;
    int a = regs_.a;

	if (isFlagSet(Flag::NEG_FLAG)) {
		if (isFlagSet(Flag::HALFC_FLAG)) {
			a = (a - 6) & 0xFF;
		}
		if (isFlagSet(Flag::CARRY_FLAG)) {
			a -= 0x60;
		}
	} else {

		if (isFlagSet(Flag::HALFC_FLAG) || ((a & 0xF) > 9)) {
			a += 0x06;
		}
		if ((isFlagSet(Flag::CARRY_FLAG)) || (a > 0x9F)) {
			a += 0x60;
		}
	}

	clear_half_carry_flag();
    clear_zero_flag();

	if ((a & 0x100) == 0x100) {
		set_carry_flag();
	}

	a &= 0xFF;

	if (a == 0) {
		set_carry_flag();
	}

	regs_.a = a;
}

void Z80Cpu::rlca_0x07() {
    tickCount_ += 4;
    uint8_t bit = regs_.a & 0x80;

    regs_.a <<= 1;

    clear_flags();

	if (bit != 0x00) {
		set_carry_flag();
		regs_.a |= 0x01;
	}
	else {
		regs_.a &= ~(0x01);
	}
}

void Z80Cpu::rla_0x17() {
    tickCount_ += 4;
    uint8_t bit = regs_.a & 0x80;
    regs_.a <<= 1;

    if (isFlagSet(Flag::CARRY_FLAG)) {
        regs_.a |= 0x01;
    } else {
        regs_.a &= ~(0x01);
    }
    clear_flags();
    if (bit != 0x00) {
        set_carry_flag();
    }
}

void Z80Cpu::rrca_0x0f()
{
    tickCount_ += 4;

    uint8_t bit = regs_.a & 0x01;
    regs_.a >>= 1;

    clear_flags();
    if (bit != 0x00)
    {
        set_carry_flag();
        regs_.a |= 0x80;
    }
    else
    {
        regs_.a &= ~(0x80);
    }
}

void Z80Cpu::rra_0x1f()
{
    tickCount_ += 4;

    uint8_t bit = regs_.a & 0x01;
    regs_.a >>= 1;
    if (isFlagSet(Flag::CARRY_FLAG))
    {
        regs_.a |= 0x80;
    }
    else
    {
        regs_.a &= ~(0x80);
    }
    clear_flags();
    if (bit != 0x00)
    {
        set_carry_flag();
    }
}