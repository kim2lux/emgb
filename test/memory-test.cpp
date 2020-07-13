#include "test.hpp"

TEST_F(myTestFixture1, memory)
{
    cpu.getMemory().write8bit(0x3569, 0x3A);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0x3569], 0x3A);
    ASSERT_EQ(cpu.getMemory().mmu_.rom_[0x3569], 0x3A);

    cpu.getMemory().write8bit(0x8000, 0x35);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0x8000], 0x35);
    ASSERT_EQ(cpu.getMemory().mmu_.vram_[0], 0x35);

    cpu.getMemory().write8bit(0xFE00, 0x86);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFE00], 0x86);
    ASSERT_EQ(cpu.getMemory().mmu_.oam_[0], 0x86);

    cpu.getMemory().write8bit(0xFF80, 0x25);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFF80], 0x25);
    ASSERT_EQ(cpu.getMemory().mmu_.hram_[0], 0x25);

    cpu.getMemory().write8bit(0xFF00, 0xAD);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFF00], 0xAD);
    ASSERT_EQ(cpu.getMemory().mmu_.io_registers_[0], 0xAD);

    cpu.getMemory().write8bit(0xFF7F, 0xBF);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFF7F], 0xBF);
    ASSERT_EQ(cpu.getMemory().mmu_.io_registers_[0x7F], 0xBF);

    cpu.getMemory().write8bit(0xFF80, 0x1f);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFF80], 0x1f);
    ASSERT_EQ(cpu.getMemory().mmu_.hram_[0], 0x1f);

    cpu.getMemory().write8bit(0xFFFE, 0x1f);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFFFE], 0x1f);
    ASSERT_EQ(cpu.getMemory().mmu_.hram_[0x7E], 0x1f);

    cpu.getMemory().write8bit(0xFFFF, 0xAA);
    ASSERT_EQ(cpu.getMemory().mmu_.raw[0xFFFF], 0xAA);
    ASSERT_EQ(cpu.getMemory().mmu_.IE_register, 0xAA);

    size_t memSize =  sizeof(cpu.getMemory().mmu_);
    ASSERT_EQ(memSize, 65536);

}
