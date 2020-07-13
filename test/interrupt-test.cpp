#include "test.hpp"
#include "utils.h"
#include "interrupt.hpp"

TEST_F(myTestFixture1, interrupt)
{
    // ENABLE / DISABLE MASTER INTERRUPT
    cpu.getInterrupt().masterInterrupt_ = true;

    cpu.opcodes_[0xF3].opFunc();
    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, false);
    cpu.opcodes_[0xFB].opFunc();
    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, true);
    
    cpu.regs_.pc = 0x2000;

    // ENABLE INTERRUPT
    cpu.getInterrupt().interruptEnable_ = cpu.getMemory().read8bit(IE_REGISTER);
    std::cout << std::hex << (int32_t)cpu.getInterrupt().interruptEnable_ << std::endl;
    setBit(cpu.getInterrupt().interruptEnable_, InterruptType::VBLANCK);
    ASSERT_EQ(cpu.getInterrupt().interruptEnable_, 0x01);
    cpu.getMemory().write8bit(IE_REGISTER, cpu.getInterrupt().interruptEnable_);

    cpu.processRequestInterrupt();

    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, true);
    ASSERT_EQ(cpu.regs_.pc, 0x2000);

    // REQUEST INTERRUPT
    cpu.getInterrupt().interruptRequest_ = cpu.getMemory().read8bit(IF_REGISTER);
    setBit(cpu.getInterrupt().interruptRequest_, InterruptType::VBLANCK);
    ASSERT_EQ(cpu.getInterrupt().interruptRequest_, 0x01);
    cpu.getMemory().write8bit(IF_REGISTER, cpu.getInterrupt().interruptRequest_);
    cpu.halt_ = true;
    cpu.processRequestInterrupt();

    ASSERT_EQ(cpu.getInterrupt().interruptRequest_, 0x00);
    ASSERT_EQ(cpu.getInterrupt().interruptEnable_, 0x01);
    ASSERT_EQ(cpu.halt_, false);
    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, false);
    ASSERT_EQ(cpu.regs_.pc, InterruptAddr::VBLANCK_JMP);
    ASSERT_EQ(isBitSet(cpu.getInterrupt().interruptRequest_, InterruptType::VBLANCK), false);
    ASSERT_EQ(cpu.getInterrupt().masterInterrupt_, false);

    // REQUEST INTERRUPT
    cpu.regs_.pc = 0x2000;
    cpu.getInterrupt().interruptRequest_ = cpu.getMemory().read8bit(IF_REGISTER);
    setBit(cpu.getInterrupt().interruptRequest_, InterruptType::LCDC);
    cpu.getMemory().write8bit(IF_REGISTER, cpu.getInterrupt().interruptRequest_);
    cpu.getInterrupt().masterInterrupt_ = true;
    cpu.processRequestInterrupt();
    ASSERT_EQ(cpu.regs_.pc, 0x2000);

    cpu.getInterrupt().interruptEnable_ = cpu.getMemory().read8bit(IE_REGISTER);
    setBit(cpu.getInterrupt().interruptEnable_, InterruptType::LCDC);
    cpu.getMemory().write8bit(IE_REGISTER, cpu.getInterrupt().interruptEnable_);
    ASSERT_EQ(cpu.getInterrupt().interruptEnable_, 0x03);
    cpu.processRequestInterrupt();
    ASSERT_EQ(cpu.halt_, false);
    ASSERT_EQ(cpu.regs_.pc, InterruptAddr::LCDC_JMP);
    cpu.getInterrupt().interruptEnable_ = cpu.getMemory().read8bit(IE_REGISTER);
    cpu.getInterrupt().interruptRequest_ = cpu.getMemory().read8bit(IF_REGISTER);
    ASSERT_EQ(cpu.getInterrupt().interruptEnable_, 0x03);
    ASSERT_EQ(cpu.getInterrupt().interruptRequest_, 0x00);


}
