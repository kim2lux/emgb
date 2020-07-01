#pragma once

#include <array>
#include "cpu.h"
#include <stdint.h>
#include <functional>
#include <vector>
#include "memory.hpp"

struct OpcodeZ80
{
    uint8_t opcode;
    const char *value;
    std::function<void()> opFunc;
    uint8_t size;
};

class Z80Cpu
{
public:
    Z80Cpu(Memory &memory);
    Memory &getMmu();
    s_register regs_;
    uint32_t tickCount_;
    std::vector<OpcodeZ80> opcodes_;
    bool jmp;
    bool running;

    void set_zero_flag() { regs_.f |= 0x80; }
    void clear_zero_flag() { regs_.f &= ~(0x80); }

    void set_neg_flag() { regs_.f |= 0x40; }
    void clear_neg_flag() { regs_.f &= ~(0x40); }

    void set_half_carry_flag() { regs_.f |= 0x20; }
    void clear_half_carry_flag() { regs_.f &= ~(0x20); }

    void set_carry_flag() { regs_.f |= 0x10; }
    void clear_carry_flag() { regs_.f &= ~(0x10); }

private:
    void initRegister();

    // decrement instruction
    void dec_a();
    uint8_t dec8(uint8_t &value);

    // load instruction
    uint16_t ld_16(uint16_t addr);
    void ld_16_to_bc_0x01();
    void ld_16_to_de_0x11();
    void ld_16_to_hl_0x21();
    void ld_16_to_sp_0x31();

    void ld_a_to_bc_addr_0x02();
    void ld_a_to_de_addr_0x12();
    void ld_a_to_hl_addr_0x22();
    void ld_a_to_hl_addr_0x32();

    // inc instruction

    void inc_bc_0x03();
    void inc_de_0x13();
    void inc_hl_0x23();
    void inc_sp_0x33();

    void inc8BitRegister(uint8_t &reg);
    void inc_b_0x04();
    void inc_d_0x14();
    void inc_h_0x24();
    void inc_hl_addr_0x34();
    void nop_0x00();

    Memory &mmu_;
};
