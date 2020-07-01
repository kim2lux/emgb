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

enum Flag: uint8_t {
    CARRY_FLAG = 0,
    HALFC_FLAG = 1,
    ZERO_FLAG = 2,
    NEG_FLAG = 3
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

    void clear_flags() { regs_.f = 0; }
    bool isFlagSet(Flag f);

private:
    void initRegister();
    void nop_0x00();
    void daa_0x27();
    void scf_0x36();

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

    uint8_t ld_8(uint16_t addr); 
    void ld_8_to_b_0x06();
    void ld_8_to_d_0x16();
    void ld_8_to_h_0x26();
    void ld_8_to_hl_addr_0x36();

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

    // dec instruction
    void dec8BitRegister(uint8_t &reg);
    void dec_b_0x05();
    void dec_d_0x15();
    void dec_h_0x25();
    void dec_hl_addr_0x35();

    // rotate
    void rlca_0x07();
    void rla_0x17();

    Memory &mmu_;
};
