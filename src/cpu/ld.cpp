#include "cpu.hpp"

void Z80Cpu::ldd_addr_a_8() {
    tickCount_ += 12;
    mmu_.write8bit(0xff00 + mmu_.read8bit(regs_.pc), regs_.a);
}

void Z80Cpu::ldh_a_val_8() {
    tickCount_ += 12;
    regs_.a = mmu_.read8bit(0xff00 + mmu_.read8bit(regs_.pc));
}

void Z80Cpu::ld_a_to_c() {
    tickCount_ += 8;
    mmu_.write8bit(0xff00 + regs_.c, regs_.a);
}

void Z80Cpu::ld_addr_c_to_a() {
    tickCount_ += 8;
    regs_.a = mmu_.read16bit(0xff00 + regs_.c);
}

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

void Z80Cpu::ld_8_to_c_0x0e()
{
    regs_.c = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_e_0x1e()
{
    regs_.e = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_l_0x2e()
{
    regs_.l = ld_8(regs_.pc);
}

void Z80Cpu::ld_8_to_a_0x3e()
{
    regs_.a = ld_8(regs_.pc);
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

void Z80Cpu::ld_8_to_hl_addr_0x36()
{
    tickCount_ += 12;
    mmu_.write8bit(regs_.hl, mmu_.read8bit(regs_.pc));
}

void Z80Cpu::ld_sp_to_addr_0x08()
{
    tickCount_ += 20;
    mmu_.write16bitToAddr(mmu_.read16bit(regs_.pc), regs_.sp);
}

void Z80Cpu::ld_bc_addr_to_a_0x0a()
{
    tickCount_ += 8;
    regs_.a = mmu_.read8bit(regs_.bc);
}
void Z80Cpu::ld_de_addr_to_a_0x1a()
{
    tickCount_ += 8;
    regs_.a = mmu_.read8bit(regs_.de);
}
void Z80Cpu::ld_hl_addr_to_a_0x2a()
{
    tickCount_ += 8;
    regs_.a = mmu_.read8bit(regs_.hl);
    regs_.hl += 1;
}
void Z80Cpu::ld_hl_addr_to_a_0x3a()
{
    tickCount_ += 8;
    regs_.a = mmu_.read8bit(regs_.hl);
    regs_.hl -= 1;
}


void Z80Cpu::ld_b_b() { tickCount_ += 4; regs_.b = regs_.b; }
void Z80Cpu::ld_b_c() { tickCount_ += 4; regs_.b = regs_.c; }
void Z80Cpu::ld_b_d() { tickCount_ += 4; regs_.b = regs_.d; }
void Z80Cpu::ld_b_e() { tickCount_ += 4; regs_.b = regs_.e; }
void Z80Cpu::ld_b_h() { tickCount_ += 4; regs_.b = regs_.h; }
void Z80Cpu::ld_b_l() { tickCount_ += 4; regs_.b = regs_.l; }
void Z80Cpu::ld_b_hl() { tickCount_ += 8; regs_.b = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_b_a() { tickCount_ += 4; regs_.b = regs_.a; }

void Z80Cpu::ld_c_b() { tickCount_ += 4; regs_.c = regs_.b; }
void Z80Cpu::ld_c_c() { tickCount_ += 4; regs_.c = regs_.c; }
void Z80Cpu::ld_c_d() { tickCount_ += 4; regs_.c = regs_.d; }
void Z80Cpu::ld_c_e() { tickCount_ += 4; regs_.c = regs_.e; }
void Z80Cpu::ld_c_h() { tickCount_ += 4; regs_.c = regs_.h; }
void Z80Cpu::ld_c_l() { tickCount_ += 4; regs_.c = regs_.l; }
void Z80Cpu::ld_c_hl() { tickCount_ += 8; regs_.c = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_c_a() { tickCount_ += 4; regs_.c = regs_.a; }

void Z80Cpu::ld_d_b() { tickCount_ += 4; regs_.d = regs_.b; }
void Z80Cpu::ld_d_c() { tickCount_ += 4; regs_.d = regs_.c; }
void Z80Cpu::ld_d_d() { tickCount_ += 4; regs_.d = regs_.d; }
void Z80Cpu::ld_d_e() { tickCount_ += 4; regs_.d = regs_.e; }
void Z80Cpu::ld_d_h() { tickCount_ += 4; regs_.d = regs_.h; }
void Z80Cpu::ld_d_l() { tickCount_ += 4; regs_.d = regs_.l; }
void Z80Cpu::ld_d_hl() { tickCount_ += 8; regs_.d = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_d_a() { tickCount_ += 4; regs_.d = regs_.a; }

void Z80Cpu::ld_e_b() { tickCount_ += 4; regs_.e = regs_.b; }
void Z80Cpu::ld_e_c() { tickCount_ += 4; regs_.e = regs_.c; }
void Z80Cpu::ld_e_d() { tickCount_ += 4; regs_.e = regs_.d; }
void Z80Cpu::ld_e_e() { tickCount_ += 4; regs_.e = regs_.e; }
void Z80Cpu::ld_e_h() { tickCount_ += 4; regs_.e = regs_.h; }
void Z80Cpu::ld_e_l() { tickCount_ += 4; regs_.e = regs_.l; }
void Z80Cpu::ld_e_hl() { tickCount_ += 8; regs_.e = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_e_a() { tickCount_ += 4; regs_.e = regs_.a; }

void Z80Cpu::ld_h_b() { tickCount_ += 4; regs_.h = regs_.b; }
void Z80Cpu::ld_h_c() { tickCount_ += 4; regs_.h = regs_.c; }
void Z80Cpu::ld_h_d() { tickCount_ += 4; regs_.h = regs_.d; }
void Z80Cpu::ld_h_e() { tickCount_ += 4; regs_.h = regs_.e; }
void Z80Cpu::ld_h_h() { tickCount_ += 4; regs_.h = regs_.h; }
void Z80Cpu::ld_h_l() { tickCount_ += 4; regs_.h = regs_.l; }
void Z80Cpu::ld_h_hl() { tickCount_ += 8; regs_.h = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_h_a() { tickCount_ += 4; regs_.h = regs_.a; }

void Z80Cpu::ld_l_b() { tickCount_ += 4; regs_.l = regs_.b; }
void Z80Cpu::ld_l_c() { tickCount_ += 4; regs_.l = regs_.c; }
void Z80Cpu::ld_l_d() { tickCount_ += 4; regs_.l = regs_.d; }
void Z80Cpu::ld_l_e() { tickCount_ += 4; regs_.l = regs_.e; }
void Z80Cpu::ld_l_h() { tickCount_ += 4; regs_.l = regs_.h; }
void Z80Cpu::ld_l_l() { tickCount_ += 4; regs_.l = regs_.l; }
void Z80Cpu::ld_l_hl() { tickCount_ += 8; regs_.l = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_l_a() { tickCount_ += 4; regs_.l = regs_.a; }

void Z80Cpu::ld_hl_b() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.b); }
void Z80Cpu::ld_hl_c() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.c); }
void Z80Cpu::ld_hl_d() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.d); }
void Z80Cpu::ld_hl_e() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.e); }
void Z80Cpu::ld_hl_h() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.h); }
void Z80Cpu::ld_hl_l() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.l); }
void Z80Cpu::ld_hl_a() { tickCount_ += 8; mmu_.write8bit(regs_.hl, regs_.a); }

void Z80Cpu::ld_a_b() { tickCount_ += 4; regs_.a = regs_.b; }
void Z80Cpu::ld_a_c() { tickCount_ += 4; regs_.a = regs_.c; }
void Z80Cpu::ld_a_d() { tickCount_ += 4; regs_.a = regs_.d; }
void Z80Cpu::ld_a_e() { tickCount_ += 4; regs_.a = regs_.e; }
void Z80Cpu::ld_a_h() { tickCount_ += 4; regs_.a = regs_.h; }
void Z80Cpu::ld_a_l() { tickCount_ += 4; regs_.a = regs_.l; }
void Z80Cpu::ld_a_hl() { tickCount_ += 8; regs_.a = mmu_.read8bit(regs_.hl); }
void Z80Cpu::ld_a_a() { tickCount_ += 4; regs_.a = regs_.a; }
