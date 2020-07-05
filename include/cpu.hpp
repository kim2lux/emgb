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

enum Flag : uint8_t
{
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
    bool fjmp_;
    bool running_= true;
    bool halt_ = false;
    bool ime_ = false;
    

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

protected:
    void initRegister();

    void nop_0x00();
    void no_operation() {
        ;
    }

    // external
    void extern_0xcb();
    //rst instruction
    void rst(const uint16_t addr);
    void rst_0_0xc7();
    void rst_10_0xd7();
    void rst_20_0xe7();
    void rst_30_0xf7();

    void rst_8_0xcf();
    void rst_18_0xdf();
    void rst_28_0xef();
    void rst_38_0xff();


    // ctrl instruction
    void daa_0x27();
    void scf_0x37();
    void stop();
    void halt_proc();
    void ime_di0xf3();
    void ei_0xfb();

    // rotate instruction
    void rrca_0x0f();
    void rra_0x1f();
    void cpl_0x2f();
    void ccf_0x3f();

    // stack instruction
    void pop_bc0xc1();
    void pop_de0xd1();
    void pop_hl0xe1();
    void push_af0xf5();
    void push_bc0xc5();
    void push_de0xd5();
    void push_hl0xe5();
    void pop_af0xf1();

    // load instruction
    void ldd_addr_a_80xe0();
    void ldh_a_val_80xf0();
    void ld_a_to_c0xe2();
    void ld_addr_c_to_a0xf2();
    void ld_hl_sp_r_0xf8();
    void ld_sp_hl_0xf9();
    void ld_a_16_0xfa();
    void ld_a_addr_0xea();

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
    void ld_8_to_c_0x0e();
    void ld_8_to_e_0x1e();
    void ld_8_to_l_0x2e();
    void ld_8_to_a_0x3e();
    void ld_sp_to_addr_0x08();

    void ld_bc_addr_to_a_0x0a();
    void ld_de_addr_to_a_0x1a();
    void ld_hl_addr_to_a_0x2a();
    void ld_hl_addr_to_a_0x3a();

    void ld_b_b();
    void ld_b_c();
    void ld_b_d();
    void ld_b_e();
    void ld_b_h();
    void ld_b_l();
    void ld_b_hl();
    void ld_b_a();

    void ld_c_b();
    void ld_c_c();
    void ld_c_d();
    void ld_c_e();
    void ld_c_h();
    void ld_c_l();
    void ld_c_hl();
    void ld_c_a();

    void ld_d_b();
    void ld_d_c();
    void ld_d_d();
    void ld_d_e();
    void ld_d_h();
    void ld_d_l();
    void ld_d_hl();
    void ld_d_a();

    void ld_e_b();
    void ld_e_c();
    void ld_e_d();
    void ld_e_e();
    void ld_e_h();
    void ld_e_l();
    void ld_e_hl();
    void ld_e_a();

    void ld_h_b();
    void ld_h_c();
    void ld_h_d();
    void ld_h_e();
    void ld_h_h();
    void ld_h_l();
    void ld_h_hl();
    void ld_h_a();

    void ld_l_b();
    void ld_l_c();
    void ld_l_d();
    void ld_l_e();
    void ld_l_h();
    void ld_l_l();
    void ld_l_hl();
    void ld_l_a();

    void ld_hl_b();
    void ld_hl_c();
    void ld_hl_d();
    void ld_hl_e();
    void ld_hl_h();
    void ld_hl_l();
    void ld_hl_a();

    void ld_a_b();
    void ld_a_c();
    void ld_a_d();
    void ld_a_e();
    void ld_a_h();
    void ld_a_l();
    void ld_a_hl();
    void ld_a_a();

    // add instruction
    void add_hl_16(uint16_t);
    void add_hl_bc_0x09();
    void add_hl_de_0x19();
    void add_hl_hl_0x29();
    void add_hl_sp_0x39();

    void add8bit(uint8_t &reg, uint8_t v);
    void add_a();
    void add_b();
    void add_c();
    void add_d();
    void add_e();
    void add_h();
    void add_l();
    void add_hl();
    void add_a_8_c6();
    void add_sp_r_0xe8();

    // sbc instruction
    void sbc8bit(uint8_t &reg, uint8_t v);
    void sbc_a();
    void sbc_b();
    void sbc_c();
    void sbc_d();
    void sbc_e();
    void sbc_h();
    void sbc_l();
    void sbc_hl();
    void sbc_a_8();
    void sbc_a_8_de();



    // or instruction
    void or8bit(uint8_t &reg, uint8_t v);
    void or_a();
    void or_b();
    void or_c();
    void or_d();
    void or_e();
    void or_h();
    void or_l();
    void or_hl();
    void or_a_8_f6();


    // cp instruction
    void cp8bit(uint8_t &reg, uint8_t v);
    void cp_a();
    void cp_b();
    void cp_c();
    void cp_d();
    void cp_e();
    void cp_h();
    void cp_l();
    void cp_hl();
    void cp_a_8_fe();


    // xor instruction
    void xor8bit(uint8_t &reg, uint8_t v);
    void xor_a();
    void xor_b();
    void xor_c();
    void xor_d();
    void xor_e();
    void xor_h();
    void xor_l();
    void xor_hl();
    void xor_a_8_ee();

    // and instruction
    void and8bit(uint8_t &reg, uint8_t v);
    void and_a();
    void and_b();
    void and_c();
    void and_d();
    void and_e();
    void and_h();
    void and_l();
    void and_hl();
    void and_a_8_e6();

    // sub instruction
    void sub8bit(uint8_t &reg, uint8_t v);
    void sub_a();
    void sub_b();
    void sub_c();
    void sub_d();
    void sub_e();
    void sub_h();
    void sub_l();
    void sub_hl();
    void sub_a_8_d6();


    // adc instruction
    void adc8bit(uint8_t &reg, uint8_t v);
    void adc_a();
    void adc_b();
    void adc_c();
    void adc_d();
    void adc_e();
    void adc_h();
    void adc_l();
    void adc_hl();
    void adc_a_8_ce();

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

    uint8_t inc8(uint8_t value);
    void inc_c_0x0c();
    void inc_e_0x1c();
    void inc_l_0x2c();
    void inc_a_0x3c();

    // dec instruction
    void dec8BitRegister(uint8_t &reg);
    void dec_b_0x05();
    void dec_d_0x15();
    void dec_h_0x25();
    void dec_hl_addr_0x35();
    void dec_c_0x0d();
    void dec_e_0x1d();
    void dec_l_0x2d();
    void dec_a_0x3d();

    void dec_bc_0x0b();
    void dec_de_0x1b();
    void dec_hl_0x2b();
    void dec_sp_0x3b();

    // rotate instruction
    void rlca_0x07();
    void rla_0x17();

    // jump instruction
    void jmp_8_0x18();
    void jmp_8_if_zero_0x28();
    void jmp_8_if_carry_0x38();
    void jmp_8_if_not_zero_0x20();
    void jmp_8_if_not_carry_0x30();

    void ret_nz0xc0();
    void ret_nc0xd0();
    void ret_z0xc8();
    void ret_c0xd8();
    void ret_0xc9();
    void reti_0xd9();
    void jmp_nz0xc2();
    void jmp_nc0xd2();
    void jmp_z0xca();
    void jmp_c0xda();
    void jmp_160xc3();
    void jmp_hl_0xe9();
    void call_nz_16_0xc4();
    void call_nc_16_0xd4();
    void call_z_16_0xcc();
    void call_c_16_0xdc();
    void call_16_0xcd();

    Memory &mmu_;
};
