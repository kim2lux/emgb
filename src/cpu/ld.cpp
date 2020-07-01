#include "cpu.hpp"

uint16_t Z80Cpu::ld_16(uint16_t addr)
{
    tickCount_ += 12;
    return mmu_.read16bit(addr);
}

void Z80Cpu::ld_16_to_bc_0x01()
{
    regs_.bc = ld_16(regs_.pc);
}

void Z80Cpu::ld_16_to_de_0x11()
{
    regs_.de = ld_16(regs_.pc);
}

void Z80Cpu::ld_16_to_hl_0x21()
{
    regs_.hl = ld_16(regs_.pc);
}

void Z80Cpu::ld_16_to_sp_0x31()
{
    regs_.sp = ld_16(regs_.pc);
}

void Z80Cpu::ld_a_to_bc_addr_0x02()
{
    tickCount_ += 8;
    mmu_.write8bit(regs_.bc, regs_.a);
}

void Z80Cpu::ld_a_to_de_addr_0x12()
{
    tickCount_ += 8;
    mmu_.write8bit(regs_.de, regs_.a);
}

void Z80Cpu::ld_a_to_hl_addr_0x22()
{
    tickCount_ += 8;
    mmu_.write8bit(regs_.hl, regs_.a);
    regs_.hl += 1;
}

void Z80Cpu::ld_a_to_hl_addr_0x32()
{
    tickCount_ += 8;
    mmu_.write8bit(regs_.hl, regs_.a);
    regs_.hl -= 1;
}

uint8_t Z80Cpu::ld_8(uint16_t addr)
{
    tickCount_ += 8;
    return mmu_.read8bit(addr);
}

void Z80Cpu::ld_8_to_b_0x06()
{
    regs_.b = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_d_0x16()
{
    regs_.d = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_h_0x26()
{
    regs_.h = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_hl_addr_0x36() {
    tickCount_ += 12;
    mmu_.write8bit(regs_.hl, mmu_.read8bit(regs_.pc));
}