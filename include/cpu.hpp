#pragma once

#include <array>
#include "cpu.h"
#include <stdint.h>
#include <functional>
#include <vector>
#include "memory.hpp"

struct OpcodeZ80
{
    unsigned char opcode;
    const char *value;
    std::function<void()> opFunc;
    unsigned char size;
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

    void set_zero() { regs_.f |= 0x80; }
    void clear_zero() { regs_.f &= ~(0x80); }

    void set_neg() { regs_.f |= 0x40; }
    void clear_neg() { regs_.f &= ~(0x40); }

    void set_half_carry() { regs_.f |= 0x20; }
    void clear_half_carry() { regs_.f &= ~(0x20); }

    void set_carry() { regs_.f |= 0x10; }
    void clear_carry() { regs_.f &= ~(0x10); }

private:

    // decrement instruction
    void dec_a();
    uint8_t dec8(uint8_t &value);

    // load instruction
    void ld_bc_val_16();

    void nop();

    Memory &memory_;
};

