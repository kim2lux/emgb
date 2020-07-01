#include "gtest/gtest.h"
#include "cpu.hpp"
#include "rom.hpp"
#include "memory"
#include <algorithm>

class myTestFixture1 : public testing::Test
{
protected:
    Cartridge cart;
    Memory mem;
    Z80Cpu cpu;
    myTestFixture1() : mem(cart), cpu(mem)
    {
        cpu.tickCount_ = 0;
        cpu.regs_.bc = 0x0000;
        cpu.regs_.de = 0x1000;
        cpu.regs_.hl = 0x2000;
        cpu.regs_.sp = 0x3000;
    }

    virtual void SetUp() override
    {
        mem.write16bitToAddr(0xC000, 0xff00);
        mem.write16bitToAddr(0xC002, 0xabcd);
        mem.write16bitToAddr(0xC004, 0x4567);
        mem.write16bitToAddr(0xC006, 0xa45d);
    }
};

TEST_F(myTestFixture1, ld8)
{
    cpu.regs_.b = 0x20;
    cpu.getMmu().write8bit(0x8000, 0x30);
    cpu.regs_.pc = 0x8000;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x06;
    })->opFunc();

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x16;
    })->opFunc();

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x26;
    })->opFunc();

    ASSERT_EQ(cpu.regs_.h, 0x30);
    cpu.regs_.hl = 0x8010;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x36;
    })->opFunc();

    ASSERT_EQ(cpu.regs_.b, 0x30);
    ASSERT_EQ(cpu.regs_.d, 0x30);
    ASSERT_EQ(cpu.getMmu().read8bit(cpu.regs_.hl), 0x30);
}

TEST_F(myTestFixture1, rotate)
{
    cpu.regs_.a = 0x01;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x07;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.a, 2);
    cpu.regs_.a = 0x80;
    cpu.clear_flags();
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), false);
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [&](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x07;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.a, 1);
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), true);

    cpu.regs_.a = 0x80;
    cpu.clear_flags();
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), false);
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [&](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x17;
    })->opFunc();
    ASSERT_NE(cpu.regs_.a, 1);
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), true);

    cpu.regs_.a = 0x10;
    cpu.set_carry_flag();
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), true);
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [&](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x17;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.a, 0x21);
    ASSERT_EQ(cpu.isFlagSet(Flag::CARRY_FLAG), false);

}


TEST_F(myTestFixture1, dec)
{
    cpu.regs_.b = 0x01;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x05;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.b, 0x00);

    cpu.regs_.d = 0x0d;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x15;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.d, 0x0c);

    cpu.regs_.h = 0x0d;
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x25;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.h, 0x0c);

    cpu.regs_.hl = 0xc000;
    cpu.getMmu().write16bitToAddr(cpu.regs_.hl, 0x4003);
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x35;
    })->opFunc();
    ASSERT_EQ(cpu.getMmu().read16bit(cpu.regs_.hl), 0x4002);
}

TEST_F(myTestFixture1, inc)
{
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x03;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.bc, 0x0001);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x13;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.de, 0x1001);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x23;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.hl, 0x2001);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x33;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.sp, 0x3001);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x04;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.b, 0x01);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x14;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.d, 0x11);

    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x24;
    })->opFunc();
    ASSERT_EQ(cpu.regs_.h, 0x21);

    cpu.regs_.hl = 0xc000;
    cpu.getMmu().write16bitToAddr(cpu.regs_.hl, 0x4003);
    std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
        return opcode.opcode == 0x34;
    })->opFunc();
    ASSERT_EQ(cpu.getMmu().read16bit(cpu.regs_.hl), 0x4004);
}

TEST_F(myTestFixture1, ld)
{
    {
        cpu.regs_.pc = 0xC000;
        cpu.regs_.bc = 0x00;
        cpu.opcodes_[0x01].opFunc();
        ASSERT_EQ(cpu.regs_.bc, 0xff00);
        ASSERT_EQ(cpu.tickCount_, 12);

        cpu.regs_.pc += 2;
        cpu.regs_.de = 0x00;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x11;
        })->opFunc();
        ASSERT_EQ(cpu.regs_.de, 0xabcd);
        ASSERT_EQ(cpu.tickCount_, 24);

        cpu.regs_.pc += 2;
        cpu.regs_.hl = 0x00;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x21;
        })->opFunc();
        ASSERT_EQ(cpu.regs_.hl, 0x4567);
        ASSERT_EQ(cpu.tickCount_, 36);

        cpu.regs_.pc += 2;
        cpu.regs_.sp = 0x00;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x31;
        })->opFunc();
        ASSERT_EQ(cpu.regs_.sp, 0xa45d);
        ASSERT_EQ(cpu.tickCount_, 48);

        cpu.regs_.bc = 0xc050;
        cpu.regs_.a = 0xf8;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x02;
        })->opFunc();
        ASSERT_EQ(cpu.getMmu().read8bit(cpu.regs_.bc), 0xf8);
        ASSERT_EQ(cpu.tickCount_, 56);

        cpu.regs_.de = 0xc050;
        cpu.regs_.a = 0x11;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x12;
        })->opFunc();
        ASSERT_EQ(cpu.getMmu().read8bit(cpu.regs_.de), 0x11);
        ASSERT_EQ(cpu.tickCount_, 64);

        cpu.regs_.hl = 0xc050;
        cpu.regs_.a = 0x12;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x22;
        })->opFunc();
        ASSERT_EQ(cpu.getMmu().read8bit(cpu.regs_.hl - 1), 0x12);
        ASSERT_EQ(cpu.tickCount_, 72);

        cpu.regs_.hl = 0xc050;
        cpu.regs_.a = 0x13;
        std::find_if(cpu.opcodes_.begin(), cpu.opcodes_.end(), [](const OpcodeZ80 &opcode) {
            return opcode.opcode == 0x32;
        })->opFunc();
        ASSERT_EQ(cpu.getMmu().read8bit(cpu.regs_.hl + 1), 0x13);
        ASSERT_EQ(cpu.tickCount_, 80);
    }
}

TEST_F(myTestFixture1, nop)
{
    cpu.regs_.a = 5;
    cpu.opcodes_[0].opFunc();
    ASSERT_EQ(cpu.regs_.a, 5);
}

TEST_F(myTestFixture1, register)
{
    {
        cpu.set_carry_flag();
        ASSERT_EQ(cpu.regs_.f & 0x10, 0x10);
        cpu.clear_carry_flag();
        ASSERT_EQ(cpu.regs_.f & 0x10, 0x00);
    }

    {
        cpu.set_half_carry_flag();
        ASSERT_EQ(cpu.regs_.f & 0x20, 0x20);
        cpu.clear_half_carry_flag();
        ASSERT_EQ(cpu.regs_.f & 0x20, 0x00);
    }

    {
        cpu.set_neg_flag();
        ASSERT_EQ(cpu.regs_.f & 0x40, 0x40);
        cpu.clear_neg_flag();
        ASSERT_EQ(cpu.regs_.f & 0x40, 0x00);
    }

    {
        cpu.set_zero_flag();
        ASSERT_EQ(cpu.regs_.f & 0x80, 0x80);
        cpu.clear_zero_flag();
        ASSERT_EQ(cpu.regs_.f & 0x80, 0x00);
    }
}
