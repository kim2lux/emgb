#include "cpu.hpp"
#include "utils.h"
void Z80Cpu::nop_0x00()
{
	tickCount_ += 4;
}

void Z80Cpu::daa_0x27() 
{
    tickCount_ += 4;
    uint8_t reg = regs_.a;
    uint16_t upd = isBitSet(regs_.f, 4) ? 0x60 : 0x00;

    if (isBitSet(regs_.f, 5) || (!isBitSet(regs_.f, 6) && ((reg & 0x0F) > 9))) {
        upd |= 0x06;
    }

    if (isBitSet(regs_.f, 4) || (!isBitSet(regs_.f, 6) && (reg > 0x99))) {
        upd |= 0x60;
    }

    if (isBitSet(regs_.f, 6)) {
        reg = static_cast<uint8_t>(reg - upd);
    } else {
        reg = static_cast<uint8_t>(reg + upd);
    }

    if (((upd << 2) & 0x100) != 0) {
        setBit(regs_.f, 4);
    }

    clearBit(regs_.f, 5);
    if (reg == 0) {
        setBit(regs_.f, 7);
    }
    regs_.a = static_cast<uint8_t>(reg);
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