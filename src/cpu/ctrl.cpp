#include "cpu.hpp"

extern "C" void handleCb(unsigned char cbopcode, Z80Cpu &cpu);
void Z80Cpu::extern_0xcb()
{
    handleCb(mmu_.read8bit(regs_.pc), *this);
}

void Z80Cpu::halt_proc() {
    halt_ = true;
}

void Z80Cpu::ei_0xfb() {
    tickCount_ += 4;
    interrupt_.masterInterrupt_ = true;
}

void Z80Cpu::cpl_0x2f()
{
    tickCount_ += 4;
    regs_.a = ~regs_.a;
    set_neg_flag();
    set_half_carry_flag();
}

void Z80Cpu::ccf_0x3f()
{
    tickCount_ += 4;
    clear_neg_flag();
    clear_half_carry_flag();

    regs_.a = regs_.a ^ 0x10;
}

void Z80Cpu::scf_0x37()
{
    tickCount_ += 4;
    clear_neg_flag();
    clear_half_carry_flag();
    set_carry_flag();
}

void Z80Cpu::stop()
{
    tickCount_ += 4;
    running_ = false;
}

void Z80Cpu::ime_di0xf3()
{
    tickCount_ += 4;
    interrupt_.masterInterrupt_ = false;
}
