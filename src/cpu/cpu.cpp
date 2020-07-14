#include "cpu.hpp"
#include "utils.h"
bool Z80Cpu::isFlagSet(Flag f)
{
    switch (f)
    {
    case Flag::CARRY_FLAG:
        return (regs_.f & 0x10);
    case Flag::ZERO_FLAG:
        return (regs_.f & 0x80);
    case Flag::NEG_FLAG:
        return (regs_.f & 0x40);
    case Flag::HALFC_FLAG:
        return (regs_.f & 0x20);
    }
    return false;
}

void Z80Cpu::set_zero_flag() { regs_.f |= 0x80; }
void Z80Cpu::clear_zero_flag() { regs_.f &= ~(0x80); }

void Z80Cpu::set_neg_flag() { regs_.f |= 0x40; }
void Z80Cpu::clear_neg_flag() { regs_.f &= ~(0x40); }

void Z80Cpu::set_half_carry_flag() { regs_.f |= 0x20; }
void Z80Cpu::clear_half_carry_flag() { regs_.f &= ~(0x20); }

void Z80Cpu::set_carry_flag() { regs_.f |= 0x10; }
void Z80Cpu::clear_carry_flag() { regs_.f &= ~(0x10); }

void Z80Cpu::clear_flags() { regs_.f = 0; }
bool Z80Cpu::isFlagSet(Flag f);

void Z80Cpu::initRegister()
{
    regs_.af = 0x11b0;
    regs_.bc = 0x0013;
    regs_.de = 0x00d8;
    regs_.hl = 0x014d;
    regs_.sp = 0xfffe;
    regs_.pc = 0x100;

    timer_.clockSpeed_ = 0;
    timer_.divRegister_ = 0;
    timer_.tac_ = 0;
    timer_.tima_ = 0;
    timer_.timerCycleCounter_ = 0;
    timer_.tma_ = 0;

    interrupt_.interruptRequest_ = 0;
    interrupt_.interruptEnable_ = 0;
    interrupt_.masterInterrupt_ = false;

    fjmp_ = false;

    set_zero_flag();
    set_half_carry_flag();
    set_carry_flag();
    clear_neg_flag();
}

void Z80Cpu::updateInterrupt()
{
    interrupt_.interruptRequest_ = getMemory().read8bit(IF_REGISTER);
    interrupt_.interruptEnable_ = getMemory().read8bit(IE_REGISTER);
}

void Z80Cpu::updateClockSpeed()
{
    timer_.tac_ = mmu_.read8bit(Timer::tac_addr);
    switch (timer_.tac_ & 0x03)
    {
    case 0:
        timer_.clockSpeed_ = 4096;
        break;
    case 1:
        timer_.clockSpeed_ = 262144;
        break;
    case 2:
        timer_.clockSpeed_ = 65536;
        break;
    case 3:
        timer_.clockSpeed_ = 16384;
        break;
    }
}

void Z80Cpu::updateTimer()
{
    timer_.divRegister_ += tickCount_;
    updateClockSpeed();
    if (isBitSet(timer_.tac_, 2) == true)
    {
        timer_.timerCycleCounter_ += tickCount_;
        if (timer_.timerCycleCounter_ >= timer_.clockSpeed_)
        {

            timer_.tima_ = mmu_.read8bit(Timer::tima_addr);
            if (timer_.tima_ == 0xff)
            {
                timer_.tima_ = mmu_.read8bit(Timer::tma_addr);
                mmu_.write8bit(Timer::tima_addr, timer_.tima_); // update tma value to tima
                setBit(interrupt_.interruptRequest_, InterruptType::TIMER);
            }
            else
            {
                timer_.tima_ += 1;
                mmu_.write8bit(Timer::tima_addr, timer_.tima_);
            }
            timer_.timerCycleCounter_ -= tickCount_;
        }
    }
    if (timer_.divRegister_ >= 256)
    {
        timer_.divRegister_ -= 256;
        uint8_t inc = mmu_.read8bit(timer_.div_addr) + 1;
        mmu_.write8bit(timer_.div_addr, inc);
    }
}

void Z80Cpu::requestInterrupt(InterruptType type) {
    interrupt_.interruptRequest_ = getMemory().read8bit(IF_REGISTER);
    switch (type) {
        case InterruptType::VBLANCK: setBit(interrupt_.interruptRequest_, 0); break;
        case InterruptType::LCDC: setBit(interrupt_.interruptRequest_, 1); break;
        case InterruptType::TIMER: setBit(interrupt_.interruptRequest_, 2); break;
        case InterruptType::SERIAL: setBit(interrupt_.interruptRequest_, 3); break;
        case InterruptType::JOYPAD: setBit(interrupt_.interruptRequest_, 4); break;
    }    
    getMemory().write8bit(IF_REGISTER, interrupt_.interruptRequest_);
}

void Z80Cpu::processRequestInterrupt()
{
    updateInterrupt();
    if (interrupt_.interruptRequest_ > 0)
    {
        if (interrupt_.masterInterrupt_ == true)
        {
            for (int interrupt = 0; interrupt < 8; ++interrupt)
            {
                if (isBitSet(interrupt_.interruptEnable_, interrupt))
                { // check if interrupt is enable
                    if (isBitSet(interrupt_.interruptRequest_, interrupt))
                    { // check if interrupt is requested
                        this->getMemory().push16(regs_.sp, regs_.pc);
                        switch (interrupt)
                        {
                        case InterruptType::VBLANCK:
                            // std::cout << "VBLANK interrupt" << std::endl;
                            regs_.pc = VBLANK_ADDR;
                            break;
                        case InterruptType::LCDC:
                            // std::cout << "LCDC interrupt" << std::endl;
                            regs_.pc = LCDC_ADDR;
                            break;
                        case InterruptType::TIMER:
                            // std::cout << "TIMER interrupt" << std::endl;
                            regs_.pc = TIME_ADDR;
                            break;
                        case InterruptType::JOYPAD:
                            // std::cout << "JOYPAD interrupt" << std::endl;
                            regs_.pc = JOYPAD_ADDR;
                            break;
                        }
                        interrupt_.masterInterrupt_ = false;
                        halt_ = false;
                        tickCount_ += 20;
                        clearBit(interrupt_.interruptRequest_, interrupt);
                        getMemory().write8bit(IF_REGISTER, interrupt_.interruptRequest_);
                        return;
                    }
                }
            }
        }
        else if (halt_ == true) {
            halt_ = false;
        }
    }
}

Z80Cpu::Z80Cpu(Memory &memory) : mmu_(memory)
{
    initRegister();
    opcodes_ = {
        {0x00, "NOP", std::bind(&Z80Cpu::nop_0x00, this), 0},
        {0x01, "ld_16_to_bc_0x01", std::bind(&Z80Cpu::ld_16_to_bc_0x01, this), 2},
        {0x02, "ld_a_to_bc_addr_0x02", std::bind(&Z80Cpu::ld_a_to_bc_addr_0x02, this), 0},
        {0x03, "inc_bc_0x03", std::bind(&Z80Cpu::inc_bc_0x03, this), 0},
        {0x04, "inc_b_0x04", std::bind(&Z80Cpu::inc_b_0x04, this), 0},
        {0x05, "dec_b_0x05", std::bind(&Z80Cpu::dec_b_0x05, this), 0},
        {0x06, "ld_8_to_b_0x06", std::bind(&Z80Cpu::ld_8_to_b_0x06, this), 1},
        {0x07, "rlca_0x07", std::bind(&Z80Cpu::rlca_0x07, this), 0},
        {0x08, "ld_sp_to_addr_0x08", std::bind(&Z80Cpu::ld_sp_to_addr_0x08, this), 2},
        {0x09, "add_hl_bc_0x09", std::bind(&Z80Cpu::add_hl_bc_0x09, this), 0},
        {0x0a, "ld_bc_addr_to_a_0x0a", std::bind(&Z80Cpu::ld_bc_addr_to_a_0x0a, this), 0},
        {0x0b, "dec_bc_0x0b", std::bind(&Z80Cpu::dec_bc_0x0b, this), 0},
        {0x0c, "inc_c_0x0c", std::bind(&Z80Cpu::inc_c_0x0c, this), 0},
        {0x0d, "dec_c_0x0d", std::bind(&Z80Cpu::dec_c_0x0d, this), 0},
        {0x0e, "ld_8_to_c_0x0e", std::bind(&Z80Cpu::ld_8_to_c_0x0e, this), 1},
        {0x0f, "rrca_0x0f", std::bind(&Z80Cpu::rrca_0x0f, this), 0},
        {0x10, "stop", std::bind(&Z80Cpu::stop, this), 1},
        {0x11, "ld_16_to_de_0x11", std::bind(&Z80Cpu::ld_16_to_de_0x11, this), 2},
        {0x12, "ld_a_to_de_addr_0x12", std::bind(&Z80Cpu::ld_a_to_de_addr_0x12, this), 0},
        {0x13, "inc_bc_0x03", std::bind(&Z80Cpu::inc_de_0x13, this), 0},
        {0x14, "inc_d_0x14", std::bind(&Z80Cpu::inc_d_0x14, this), 0},
        {0x15, "dec_d_0x15", std::bind(&Z80Cpu::dec_d_0x15, this), 0},
        {0x16, "ld_8_to_d_0x16", std::bind(&Z80Cpu::ld_8_to_d_0x16, this), 1},
        {0x17, "rla_0x08", std::bind(&Z80Cpu::rla_0x17, this), 0},
        {0x18, "jmp_8_0x18", std::bind(&Z80Cpu::jmp_8_0x18, this), 1},
        {0x19, "add_hl_de_0x19", std::bind(&Z80Cpu::add_hl_de_0x19, this), 0},
        {0x1a, "ld_de_addr_to_a_0x1a", std::bind(&Z80Cpu::ld_de_addr_to_a_0x1a, this), 0},
        {0x1b, "dec_de_0x1b", std::bind(&Z80Cpu::dec_de_0x1b, this), 0},
        {0x1c, "inc_e_0x1c", std::bind(&Z80Cpu::inc_e_0x1c, this), 0},
        {0x1d, "dec_e_0x1d", std::bind(&Z80Cpu::dec_e_0x1d, this), 0},
        {0x1e, "ld_8_to_e_0x1e", std::bind(&Z80Cpu::ld_8_to_e_0x1e, this), 1},
        {0x1f, "rra_0x1f", std::bind(&Z80Cpu::rra_0x1f, this), 0},
        {0x20, "jmp_8_if_not_zero_0x20", std::bind(&Z80Cpu::jmp_8_if_not_zero_0x20, this), 1},
        {0x21, "ld_16_to_hl_0x21", std::bind(&Z80Cpu::ld_16_to_hl_0x21, this), 2},
        {0x22, "ld_a_to_hl_addr_0x22", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x22, this), 0},
        {0x23, "inc_bc_0x23", std::bind(&Z80Cpu::inc_hl_0x23, this), 0},
        {0x24, "inc_h_0x14", std::bind(&Z80Cpu::inc_h_0x24, this), 0},
        {0x25, "dec_h_0x25", std::bind(&Z80Cpu::dec_h_0x25, this), 0},
        {0x26, "ld_8_to_h_0x26", std::bind(&Z80Cpu::ld_8_to_h_0x26, this), 1},
        {0x27, "daa_0x27", std::bind(&Z80Cpu::daa_0x27, this), 0},
        {0x28, "jmp_8_if_zero_0x28", std::bind(&Z80Cpu::jmp_8_if_zero_0x28, this), 1},
        {0x29, "add_hl_hl_0x29", std::bind(&Z80Cpu::add_hl_hl_0x29, this), 0},
        {0x2a, "ld_hl_addr_to_a_0x2a", std::bind(&Z80Cpu::ld_hl_addr_to_a_0x2a, this), 0},
        {0x2b, "dec_hl_0x2b", std::bind(&Z80Cpu::dec_hl_0x2b, this), 0},
        {0x2c, "inc_l_0x2c", std::bind(&Z80Cpu::inc_l_0x2c, this), 0},
        {0x2d, "dec_l_0x2d", std::bind(&Z80Cpu::dec_l_0x2d, this), 0},
        {0x2e, "ld_8_to_l_0x2e", std::bind(&Z80Cpu::ld_8_to_l_0x2e, this), 1},
        {0x2f, "cpl_0x2f", std::bind(&Z80Cpu::cpl_0x2f, this), 0},
        {0x30, "jmp_8_if_not_carry_0x30", std::bind(&Z80Cpu::jmp_8_if_not_carry_0x30, this), 1},
        {0x31, "ld_16_to_sp_0x31", std::bind(&Z80Cpu::ld_16_to_sp_0x31, this), 2},
        {0x32, "ld_a_to_hl_addr_0x32", std::bind(&Z80Cpu::ld_a_to_hl_addr_0x32, this), 0},
        {0x33, "inc_sp_0x33", std::bind(&Z80Cpu::inc_sp_0x33, this), 0},
        {0x34, "inc_hl_addr_0x34", std::bind(&Z80Cpu::inc_hl_addr_0x34, this), 0},
        {0x35, "dec_hl_addr_0x35", std::bind(&Z80Cpu::dec_hl_addr_0x35, this), 0},
        {0x36, "ld_8_to_hl_addr_0x36", std::bind(&Z80Cpu::ld_8_to_hl_addr_0x36, this), 1},
        {0x37, "scf_0x37", std::bind(&Z80Cpu::scf_0x37, this), 0},
        {0x38, "jmp_8_if_carry_0x38", std::bind(&Z80Cpu::jmp_8_if_carry_0x38, this), 1},
        {0x39, "add_hl_sp_0x39", std::bind(&Z80Cpu::add_hl_sp_0x39, this), 0},
        {0x3a, "ld_hl_addr_to_a_0x3a", std::bind(&Z80Cpu::ld_hl_addr_to_a_0x3a, this), 0},
        {0x3b, "dec_sp_0x3b", std::bind(&Z80Cpu::dec_sp_0x3b, this), 0},
        {0x3c, "inc_a_0x3c", std::bind(&Z80Cpu::inc_a_0x3c, this), 0},
        {0x3d, "dec_a_0x3d", std::bind(&Z80Cpu::dec_a_0x3d, this), 0},
        {0x3e, "ld_8_to_a_0x3e", std::bind(&Z80Cpu::ld_8_to_a_0x3e, this), 1},
        {0x3f, "ccf_0x3f", std::bind(&Z80Cpu::ccf_0x3f, this), 0},

        // 0x40 -> 0x7f
        {0x40, "ld b <- b", std::bind(&Z80Cpu::ld_b_b, this), 0}, //b
        {0x41, "ld b <- c", std::bind(&Z80Cpu::ld_b_c, this), 0},
        {0x42, "ld b <- d", std::bind(&Z80Cpu::ld_b_d, this), 0},
        {0x43, "ld b <- e", std::bind(&Z80Cpu::ld_b_e, this), 0},
        {0x44, "ld b <- h", std::bind(&Z80Cpu::ld_b_h, this), 0},
        {0x45, "ld b <- l", std::bind(&Z80Cpu::ld_b_l, this), 0},
        {0x46, "ld b <- hl", std::bind(&Z80Cpu::ld_b_hl, this), 0},
        {0x47, "ld b <- a", std::bind(&Z80Cpu::ld_b_a, this), 0},

        {0x48, "ld c <- b", std::bind(&Z80Cpu::ld_c_b, this), 0}, //c
        {0x49, "ld c <- c", std::bind(&Z80Cpu::ld_c_c, this), 0},
        {0x4a, "ld c <- d", std::bind(&Z80Cpu::ld_c_d, this), 0},
        {0x4b, "ld c <- e", std::bind(&Z80Cpu::ld_c_e, this), 0},
        {0x4c, "ld c <- h", std::bind(&Z80Cpu::ld_c_h, this), 0},
        {0x4d, "ld c <- l", std::bind(&Z80Cpu::ld_c_l, this), 0},
        {0x4e, "ld c <- hl", std::bind(&Z80Cpu::ld_c_hl, this), 0},
        {0x4f, "ld c <- a", std::bind(&Z80Cpu::ld_c_a, this), 0},

        {0x50, "ld d <- b", std::bind(&Z80Cpu::ld_d_b, this), 0}, //D
        {0x51, "ld d <- c", std::bind(&Z80Cpu::ld_d_c, this), 0},
        {0x52, "ld d <- d", std::bind(&Z80Cpu::ld_d_d, this), 0},
        {0x53, "ld d <- e", std::bind(&Z80Cpu::ld_d_e, this), 0},
        {0x54, "ld d <- h", std::bind(&Z80Cpu::ld_d_h, this), 0},
        {0x55, "ld d <- l", std::bind(&Z80Cpu::ld_d_l, this), 0},
        {0x56, "ld d <- hl", std::bind(&Z80Cpu::ld_d_hl, this), 0},
        {0x57, "ld d <- a", std::bind(&Z80Cpu::ld_d_a, this), 0},

        {0x58, "ld e <- b", std::bind(&Z80Cpu::ld_e_b, this), 0}, //E
        {0x59, "ld e <- c", std::bind(&Z80Cpu::ld_e_c, this), 0},
        {0x5a, "ld e <- d", std::bind(&Z80Cpu::ld_e_d, this), 0},
        {0x5b, "ld e <- e", std::bind(&Z80Cpu::ld_e_e, this), 0},
        {0x5c, "ld e <- h", std::bind(&Z80Cpu::ld_e_h, this), 0},
        {0x5d, "ld e <- l", std::bind(&Z80Cpu::ld_e_l, this), 0},
        {0x5e, "ld e <- hl", std::bind(&Z80Cpu::ld_e_hl, this), 0},
        {0x5f, "ld e <- a", std::bind(&Z80Cpu::ld_e_a, this), 0},

        {0x60, "ld h <- b", std::bind(&Z80Cpu::ld_h_b, this), 0}, //H
        {0x61, "ld h <- c", std::bind(&Z80Cpu::ld_h_c, this), 0},
        {0x62, "ld h <- d", std::bind(&Z80Cpu::ld_h_d, this), 0},
        {0x63, "ld h <- e", std::bind(&Z80Cpu::ld_h_e, this), 0},
        {0x64, "ld h <- h", std::bind(&Z80Cpu::ld_h_h, this), 0},
        {0x65, "ld h <- l", std::bind(&Z80Cpu::ld_h_l, this), 0},
        {0x66, "ld h <- hl", std::bind(&Z80Cpu::ld_h_hl, this), 0},
        {0x67, "ld h <- a", std::bind(&Z80Cpu::ld_h_a, this), 0},

        {0x68, "ld l <- b", std::bind(&Z80Cpu::ld_l_b, this), 0}, //L
        {0x69, "ld l <- c", std::bind(&Z80Cpu::ld_l_c, this), 0},
        {0x6a, "ld l <- d", std::bind(&Z80Cpu::ld_l_d, this), 0},
        {0x6b, "ld l <- e", std::bind(&Z80Cpu::ld_l_e, this), 0},
        {0x6c, "ld l <- h", std::bind(&Z80Cpu::ld_l_h, this), 0},
        {0x6d, "ld l <- l", std::bind(&Z80Cpu::ld_l_l, this), 0},
        {0x6e, "ld l <- hl", std::bind(&Z80Cpu::ld_l_hl, this), 0},
        {0x6f, "ld l <- a", std::bind(&Z80Cpu::ld_l_a, this), 0},

        {0x70, "ld hl <- b", std::bind(&Z80Cpu::ld_hl_b, this), 0}, //(HL)
        {0x71, "ld hl <- c", std::bind(&Z80Cpu::ld_hl_c, this), 0},
        {0x72, "ld hl <- d", std::bind(&Z80Cpu::ld_hl_d, this), 0},
        {0x73, "ld hl <- e", std::bind(&Z80Cpu::ld_hl_e, this), 0},
        {0x74, "ld hl <- h", std::bind(&Z80Cpu::ld_hl_h, this), 0},
        {0x75, "ld hl <- l", std::bind(&Z80Cpu::ld_hl_l, this), 0},
        {0x76, "halt proc", std::bind(&Z80Cpu::halt_proc, this), 0},
        {0x77, "ld hl <- a", std::bind(&Z80Cpu::ld_hl_a, this), 0},

        {0x78, "ld a <- b", std::bind(&Z80Cpu::ld_a_b, this), 0}, //A
        {0x79, "ld a <- c", std::bind(&Z80Cpu::ld_a_c, this), 0},
        {0x7a, "ld a <- d", std::bind(&Z80Cpu::ld_a_d, this), 0},
        {0x7b, "ld a <- e", std::bind(&Z80Cpu::ld_a_e, this), 0},
        {0x7c, "ld a <- h", std::bind(&Z80Cpu::ld_a_h, this), 0},
        {0x7d, "ld a <- l", std::bind(&Z80Cpu::ld_a_l, this), 0},
        {0x7e, "ld a <- hl", std::bind(&Z80Cpu::ld_a_hl, this), 0},
        {0x7f, "ld a <- a", std::bind(&Z80Cpu::ld_a_a, this), 0},

        {0x80, "add a <- b", std::bind(&Z80Cpu::add_b, this), 0},
        {0x81, "add a <- c", std::bind(&Z80Cpu::add_c, this), 0},
        {0x82, "add a <- d", std::bind(&Z80Cpu::add_d, this), 0},
        {0x83, "add a <- e", std::bind(&Z80Cpu::add_e, this), 0},
        {0x84, "add a <- h", std::bind(&Z80Cpu::add_h, this), 0},
        {0x85, "add a <- l", std::bind(&Z80Cpu::add_l, this), 0},
        {0x86, "add a <- (hl)", std::bind(&Z80Cpu::add_hl, this), 0},
        {0x87, "add a <- a", std::bind(&Z80Cpu::add_a, this), 0},

        {0x88, "adc a b", std::bind(&Z80Cpu::adc_b, this), 0},
        {0x89, "adc a c", std::bind(&Z80Cpu::adc_c, this), 0},
        {0x8a, "adc a d", std::bind(&Z80Cpu::adc_d, this), 0},
        {0x8b, "adc a e", std::bind(&Z80Cpu::adc_e, this), 0},
        {0x8c, "adc a h", std::bind(&Z80Cpu::adc_h, this), 0},
        {0x8d, "adc a l", std::bind(&Z80Cpu::adc_l, this), 0},
        {0x8e, "adc a (hl)", std::bind(&Z80Cpu::adc_hl, this), 0},
        {0x8f, "adc a a", std::bind(&Z80Cpu::adc_a, this), 0},

        {0x90, "sub_b", std::bind(&Z80Cpu::sub_b, this), 0},
        {0x91, "sub_c", std::bind(&Z80Cpu::sub_c, this), 0},
        {0x92, "sub_d", std::bind(&Z80Cpu::sub_d, this), 0},
        {0x93, "sub_e", std::bind(&Z80Cpu::sub_e, this), 0},
        {0x94, "sub_h", std::bind(&Z80Cpu::sub_h, this), 0},
        {0x95, "sub_l", std::bind(&Z80Cpu::sub_l, this), 0},
        {0x96, "sub_hl", std::bind(&Z80Cpu::sub_hl, this), 0},

        {0x97, "sub_a", std::bind(&Z80Cpu::sub_a, this), 0},
        {0x98, "sbc b", std::bind(&Z80Cpu::sbc_b, this), 0},
        {0x99, "sbc c", std::bind(&Z80Cpu::sbc_c, this), 0},
        {0x9a, "sbc d", std::bind(&Z80Cpu::sbc_d, this), 0},
        {0x9b, "sbc e", std::bind(&Z80Cpu::sbc_e, this), 0},
        {0x9c, "sbc h", std::bind(&Z80Cpu::sbc_h, this), 0},
        {0x9d, "sbc l", std::bind(&Z80Cpu::sbc_l, this), 0},
        {0x9e, "sbc hl", std::bind(&Z80Cpu::sbc_hl, this), 0},
        {0x9f, "sbc a", std::bind(&Z80Cpu::sbc_a, this), 0},

        {0xa0, "and b", std::bind(&Z80Cpu::and_b, this), 0},
        {0xa1, "and c", std::bind(&Z80Cpu::and_c, this), 0},
        {0xa2, "and d", std::bind(&Z80Cpu::and_d, this), 0},
        {0xa3, "and e", std::bind(&Z80Cpu::and_e, this), 0},
        {0xa4, "and h", std::bind(&Z80Cpu::and_h, this), 0},
        {0xa5, "and l", std::bind(&Z80Cpu::and_l, this), 0},
        {0xa6, "and hl", std::bind(&Z80Cpu::and_hl, this), 0},
        {0xa7, "and a", std::bind(&Z80Cpu::and_a, this), 0},

        {0xa8, "xor b against a", std::bind(&Z80Cpu::xor_b, this), 0},
        {0xa9, "xor c against a", std::bind(&Z80Cpu::xor_c, this), 0},
        {0xaa, "xor d against a", std::bind(&Z80Cpu::xor_d, this), 0},
        {0xab, "xor e against a", std::bind(&Z80Cpu::xor_e, this), 0},
        {0xac, "xor h against a", std::bind(&Z80Cpu::xor_h, this), 0},
        {0xad, "xor l against a", std::bind(&Z80Cpu::xor_l, this), 0},
        {0xae, "xor hl against a", std::bind(&Z80Cpu::xor_hl, this), 0},
        {0xaf, "xor A against a", std::bind(&Z80Cpu::xor_a, this), 0},

        {0xb0, "or_b", std::bind(&Z80Cpu::or_b, this), 0},
        {0xb1, "or_c", std::bind(&Z80Cpu::or_c, this), 0},
        {0xb2, "or_d", std::bind(&Z80Cpu::or_d, this), 0},
        {0xb3, "or_e", std::bind(&Z80Cpu::or_e, this), 0},
        {0xb4, "or_h", std::bind(&Z80Cpu::or_h, this), 0},
        {0xb5, "or_l", std::bind(&Z80Cpu::or_l, this), 0},
        {0xb6, "or_hl", std::bind(&Z80Cpu::or_hl, this), 0},
        {0xb7, "or_a", std::bind(&Z80Cpu::or_a, this), 0},

        {0xb8, "cp b", std::bind(&Z80Cpu::cp_b, this), 0},
        {0xb9, "cp c", std::bind(&Z80Cpu::cp_c, this), 0},
        {0xba, "cp d", std::bind(&Z80Cpu::cp_d, this), 0},
        {0xbb, "cp e", std::bind(&Z80Cpu::cp_e, this), 0},
        {0xbc, "cp h", std::bind(&Z80Cpu::cp_h, this), 0},
        {0xbd, "cp l", std::bind(&Z80Cpu::cp_l, this), 0},
        {0xbe, "cp (hl)", std::bind(&Z80Cpu::cp_hl, this), 0},
        {0xbf, "cp a", std::bind(&Z80Cpu::cp_a, this), 0},

        {0xc0, "ret_nz0xc0", std::bind(&Z80Cpu::ret_nz0xc0, this), 0},
        {0xc1, "pop_bc0xc1", std::bind(&Z80Cpu::pop_bc0xc1, this), 0},
        {0xc2, "jmp_nz", std::bind(&Z80Cpu::jmp_nz0xc2, this), 2},
        {0xc3, "jmp_16", std::bind(&Z80Cpu::jmp_160xc3, this), 2},
        {0xc4, "call_nz_16_0xc4", std::bind(&Z80Cpu::call_nz_16_0xc4, this), 2},
        {0xc5, "push_bc", std::bind(&Z80Cpu::push_bc0xc5, this), 0},
        {0xc6, "add_a_8_c6", std::bind(&Z80Cpu::add_a_8_c6, this), 1},
        {0xc7, "rst_0_0xc7", std::bind(&Z80Cpu::rst_0_0xc7, this), 0},
        {0xc8, "ret z", std::bind(&Z80Cpu::ret_z0xc8, this), 0},
        {0xc9, "ret_0xc9", std::bind(&Z80Cpu::ret_0xc9, this), 0},
        {0xca, "jmp_z", std::bind(&Z80Cpu::jmp_z0xca, this), 2},
        {0xcb, "extern_0xcb", std::bind(&Z80Cpu::extern_0xcb, this), 1},
        {0xcc, "call_z_16_0xcc", std::bind(&Z80Cpu::call_z_16_0xcc, this), 2},
        {0xcd, "call_16_0xcd", std::bind(&Z80Cpu::call_16_0xcd, this), 2},
        {0xce, "adc_a_8_ce", std::bind(&Z80Cpu::adc_a_8_ce, this), 1},
        {0xcf, "rst_8_0xcf", std::bind(&Z80Cpu::rst_8_0xcf, this), 0},
        {0xd0, "ret nc", std::bind(&Z80Cpu::ret_nc0xd0, this), 0},
        {0xd1, "pop_de", std::bind(&Z80Cpu::pop_de0xd1, this), 0},
        {0xd2, "jmp_nc", std::bind(&Z80Cpu::jmp_nc0xd2, this), 2},
        {0xd3, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xd4, "call_nc_16_0xd4", std::bind(&Z80Cpu::call_nc_16_0xd4, this), 2},
        {0xd5, "push_de", std::bind(&Z80Cpu::push_de0xd5, this), 0},
        {0xd6, "sub_a_8_d6", std::bind(&Z80Cpu::sub_a_8_d6, this), 1},
        {0xd7, "rst_10_0xd7", std::bind(&Z80Cpu::rst_10_0xd7, this), 0},
        {0xd8, "ret c", std::bind(&Z80Cpu::ret_c0xd8, this), 0},
        {0xd9, "reti_0xd9", std::bind(&Z80Cpu::reti_0xd9, this), 0},
        {0xda, "jmp_c", std::bind(&Z80Cpu::jmp_c0xda, this), 2},
        {0xdb, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xdc, "call_c_16_0xcc", std::bind(&Z80Cpu::call_c_16_0xdc, this), 2},
        {0xdd, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xde, "sbc_a_8_de", std::bind(&Z80Cpu::sbc_a_8_de, this), 1},
        {0xdf, "rst_18_0xdf", std::bind(&Z80Cpu::rst_18_0xdf, this), 0},
        {0xe0, "ld a to addr", std::bind(&Z80Cpu::ldd_addr_a_80xe0, this), 1},
        {0xe1, "pop_hl", std::bind(&Z80Cpu::pop_hl0xe1, this), 0},
        {0xe2, "ld_a_to_c", std::bind(&Z80Cpu::ld_a_to_c0xe2, this), 0},
        {0xe3, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xe4, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xe5, "push_hl", std::bind(&Z80Cpu::push_hl0xe5, this), 0},
        {0xe6, "and_a_8_e6", std::bind(&Z80Cpu::and_a_8_e6, this), 1},
        {0xe7, "rst_20_0xe7", std::bind(&Z80Cpu::rst_20_0xe7, this), 0},
        {0xe8, "add_sp_r_0xe8", std::bind(&Z80Cpu::add_sp_r_0xe8, this), 1},
        {0xe9, "jmp_hl_0xe9", std::bind(&Z80Cpu::jmp_hl_0xe9, this), 0},
        {0xea, "ld_a_addr_0xea", std::bind(&Z80Cpu::ld_a_addr_0xea, this), 2},
        {0xeb, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xec, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xed, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xee, "xor_a_8_ee", std::bind(&Z80Cpu::xor_a_8_ee, this), 1},
        {0xef, "rst_28_0xef", std::bind(&Z80Cpu::rst_28_0xef, this), 0},
        {0xf0, "ldh_a_val_8", std::bind(&Z80Cpu::ldh_a_val_80xf0, this), 1},
        {0xf1, "pop_af", std::bind(&Z80Cpu::pop_af0xf1, this), 0},
        {0xf2, "ld_addr_c_to_a", std::bind(&Z80Cpu::ld_addr_c_to_a0xf2, this), 0},
        {0xf3, "ime_di", std::bind(&Z80Cpu::ime_di0xf3, this), 0},
        {0xf4, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xf5, "push_af", std::bind(&Z80Cpu::push_af0xf5, this), 0},
        {0xf6, "or_a_8_f6", std::bind(&Z80Cpu::or_a_8_f6, this), 1},
        {0xf7, "rst_20_0xe7", std::bind(&Z80Cpu::rst_30_0xf7, this), 0},
        {0xf8, "ld_hl_sp_r_0xf8", std::bind(&Z80Cpu::ld_hl_sp_r_0xf8, this), 1},
        {0xf9, "ld_sp_hl_0xf9", std::bind(&Z80Cpu::ld_sp_hl_0xf9, this), 0},
        {0xfa, "ld_a_16_0xfa", std::bind(&Z80Cpu::ld_a_16_0xfa, this), 2},
        {0xfb, "ei_0xfb", std::bind(&Z80Cpu::ei_0xfb, this), 0},
        {0xfc, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xfd, "no_operation", std::bind(&Z80Cpu::no_operation, this), 0},
        {0xfe, "cp_a_8_fe", std::bind(&Z80Cpu::cp_a_8_fe, this), 1},
        {0xff, "rst_38_0xf7", std::bind(&Z80Cpu::rst_38_0xff, this), 0},
    };
}

Memory &Z80Cpu::getMemory()
{
    return mmu_;
}
