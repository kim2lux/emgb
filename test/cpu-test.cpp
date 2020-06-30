#include "gtest/gtest.h"
#include "cpu.hpp"
#include "rom.hpp"
#include "memory"
TEST(cpuTest, nop)
{
    //arrange
    //act
    //assert
    Cartridge cart;
    Memory mem(cart);
    Z80Cpu cpu(mem);
    cpu.regs_.a = 5;
    cpu.opcodes_[0].opFunc();
    ASSERT_EQ(cpu.regs_.a, 5);
}

TEST(cpuTest, register)
{
    //arrange
    //act
    //assert
    Cartridge cart;
    Memory mem(cart);
    Z80Cpu cpu(mem);
    cpu.regs_.f = 0;
    {
        cpu.set_carry();
        ASSERT_EQ(cpu.regs_.f & 0x10, 0x10);
        cpu.clear_carry();
        ASSERT_EQ(cpu.regs_.f & 0x10, 0x00);
    }

    {
        cpu.set_half_carry();
        ASSERT_EQ(cpu.regs_.f & 0x20, 0x20);
        cpu.clear_half_carry();
        ASSERT_EQ(cpu.regs_.f & 0x20, 0x00);
    }

    {
        cpu.set_neg();
        ASSERT_EQ(cpu.regs_.f & 0x40, 0x40);
        cpu.clear_neg();
        ASSERT_EQ(cpu.regs_.f & 0x40, 0x00);
    }

    {
        cpu.set_zero();
        ASSERT_EQ(cpu.regs_.f & 0x80, 0x80);
        cpu.clear_zero();
        ASSERT_EQ(cpu.regs_.f & 0x80, 0x00);
    }
}
