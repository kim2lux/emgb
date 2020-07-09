#include "test.hpp"
#include "utils.h"

TEST_F(myTestFixture1, jump)
{
    // 0x18
    cpu.regs_.pc = 0xc000;
    cpu.getMemory().write8bit(cpu.regs_.pc, 0x10);
    cpu.opcodes_[0x18].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc011);

    // 0x20 set
    cpu.regs_.pc = 0xc000;
    cpu.getMemory().write8bit(cpu.regs_.pc, 0x5f);

    cpu.set_zero_flag();
    cpu.opcodes_[0x20].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.clear_zero_flag();
    cpu.opcodes_[0x20].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc060);

    // 0x30
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.getMemory().write8bit(cpu.regs_.pc, 0x5f);

    cpu.set_carry_flag();
    cpu.opcodes_[0x30].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.clear_carry_flag();
    cpu.opcodes_[0x30].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc060);

    // 0x28
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.getMemory().write8bit(cpu.regs_.pc, 0x5f);

    cpu.clear_zero_flag();
    cpu.opcodes_[0x28].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.set_zero_flag();
    cpu.opcodes_[0x28].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc060);

    // 0x38
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.getMemory().write8bit(cpu.regs_.pc, 0x5f);

    cpu.clear_carry_flag();
    cpu.opcodes_[0x38].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.set_carry_flag();
    cpu.opcodes_[0x38].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc060);

    // 0xc9
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);

    cpu.opcodes_[0xc9].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);

    // 0xd9
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getInterrupt().masterInterrupt_ = false;

    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);
    cpu.opcodes_[0xd9].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);
    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, true);

    // 0xc0
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);

    cpu.set_zero_flag();
    cpu.opcodes_[0xc0].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.clear_zero_flag();
    cpu.opcodes_[0xc0].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);

    // 0xc8
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);

    cpu.clear_zero_flag();
    cpu.opcodes_[0xc8].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.set_zero_flag();
    cpu.opcodes_[0xc8].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);

    // 0xd0
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);

    cpu.set_carry_flag();
    cpu.opcodes_[0xd0].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.clear_carry_flag();
    cpu.opcodes_[0xd0].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);

    // 0xd8
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;
    cpu.getMemory().push16(cpu.regs_.sp, 0xc689);

    cpu.clear_carry_flag();
    cpu.opcodes_[0xd8].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.set_carry_flag();
    cpu.opcodes_[0xd8].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc689);

    // 0xd4
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;

    cpu.getMemory().write16bitToAddr(cpu.regs_.pc, 0xd522);
    cpu.set_carry_flag();
    cpu.opcodes_[0xd4].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);


    cpu.clear_carry_flag();
    cpu.opcodes_[0xd4].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xd522);
    ASSERT_EQ(cpu.getMemory().pop16(cpu.regs_.sp), 0xc000+2);

    // 0xdc
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;

    cpu.getMemory().write16bitToAddr(cpu.regs_.pc, 0xd522);
    cpu.clear_carry_flag();
    cpu.opcodes_[0xdc].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);

    cpu.set_carry_flag();
    cpu.opcodes_[0xdc].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xd522);
    ASSERT_EQ(cpu.getMemory().pop16(cpu.regs_.sp), 0xc000+2);

    // 0xc4
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;

    cpu.getMemory().write16bitToAddr(cpu.regs_.pc, 0xd522);
    cpu.set_zero_flag();
    cpu.opcodes_[0xc4].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);


    cpu.clear_zero_flag();
    cpu.opcodes_[0xc4].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xd522);
    ASSERT_EQ(cpu.getMemory().pop16(cpu.regs_.sp), 0xc000+2);

    // 0xcc
    cpu.regs_.af = 0x00;
    cpu.regs_.pc = 0xc000;
    cpu.regs_.sp = 0xfffe;

    cpu.getMemory().write16bitToAddr(cpu.regs_.pc, 0xd522);
    cpu.clear_zero_flag();
    cpu.opcodes_[0xcc].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xc000);


    cpu.set_zero_flag();
    cpu.opcodes_[0xcc].opFunc();
    ASSERT_EQ(cpu.regs_.pc, 0xd522);
    ASSERT_EQ(cpu.getMemory().pop16(cpu.regs_.sp), 0xc000+2);

}
