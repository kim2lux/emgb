#pragma once
#include "cpu.hpp"
#include <stdint.h>
#include <memory>
#include <iostream>


static constexpr uint16_t IF_REGISTER = 0xFF0F;
static constexpr uint16_t IE_REGISTER = 0xFFFF;

static constexpr uint16_t TIMA_REGISTER = 0xFF05;

static constexpr uint16_t LCD_REGISTER = 0xFF40;

static constexpr uint16_t SCX_REGISTER = 0xFF03;
static constexpr uint16_t SCY_REGISTER = 0xFF02;


static constexpr uint16_t VBLANK_ADDR = 0x40;
static constexpr uint16_t LCDC_ADDR = 0x48;
static constexpr uint16_t TIME_ADDR = 0x50;
static constexpr uint16_t JOYPAD_ADDR = 0x60;

enum InterruptType {
    VBLANCK = 0x00,
    LCDC = 0x01,
    TIMER = 0x02,
    SERIAL = 0x03,
    JOYPAD = 0x04
};

enum InterruptAddr {
    VBLANCK_JMP = 0x40,
    LCDC_JMP = 0x48,
    TIMER_JMP = 0x50,
    SERIAL_JMP = 0x58,
    JOYPAD_JMP = 0x60
};

struct Interrupt
{
public:
    uint8_t interruptRequest_;
    uint8_t interruptEnable_;
    bool masterInterrupt_;
};
