#pragma once


#include "rom.hpp"
#include "joypad.hpp"
#include <memory>
#include "cpu.hpp"

static constexpr uint16_t serial_data_addr = 0xff01;
static constexpr uint16_t joypad_state_addr = 0xff00;
static constexpr uint16_t scanline_value_addr = 0xff44;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

union u_mmu
{
    struct __attribute__((packed))
    {
        uint8_t rom_[0x8000]; // 0 - 7FFF cartridge memory
        uint8_t vram_[0x2000]; // 8000 - 9FFF video RAM
        uint8_t sram_[0x2000]; // A000 - BFFF External RAM switch bank
        uint8_t ram_[0x2000]; // C000 - DFFF WRAM
        uint8_t echo_ram[0x1E00]; // E000- FDFF Echo Ram (Mapping C000~DDFF)
        uint8_t oam_[0x00A0]; // FE00 - FE9F oam information tables
        uint8_t unused_[0x0060]; // FEA0 - FEFF unused on GMB
        uint8_t io_registers_[0x80]; // FF00 - FF7F
        uint8_t hram_[0x007f]; // FF80 - FFFE
        uint8_t IE_register; // FFFF
    };
    uint8_t raw[0x10000];
};

#pragma GCC diagnostic pop

class Joypad;
class Gpu;
class Z80Cpu;
class Memory
{
public:
    Memory(Cartridge &cart, Joypad &joypad) : cart_(cart), joypad_(joypad) {
        gpu_ = nullptr;
        cpu_ = nullptr;
        memoryInit();
        testScanline_ = 0x89;
        interupt_ = 0;
    }

    void load(std::vector<uint8_t>& buffer) {
        for (uint32_t idx = 0; idx <= 0xffff; ++idx) {
            mmu_.raw[idx] = buffer[idx];
        }
    }

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> serialize;
        serialize.reserve(0xffff);
        for (uint32_t idx = 0; idx <= 0xffff; ++idx) {
            serialize.push_back(mmu_.raw[idx]);
        }
        return (serialize);
    }

    void setGpu(std::shared_ptr<Gpu> gpu);
    void setCpu(std::shared_ptr<Z80Cpu> cpu);
    std::shared_ptr<Gpu> gpu_;
    std::shared_ptr<Z80Cpu> cpu_;
    void dmaTransfer(uint8_t value);
    Cartridge &cart_;
    Joypad &joypad_;
    u_mmu mmu_;
    uint8_t testScanline_ = 0x89;
    uint8_t interupt_ = 0;

    void write8bit(uint16_t addr, uint8_t value);
    void write16bitToAddr(uint16_t addr, uint16_t value);
    uint8_t read8bit(uint16_t addr);
    uint16_t read16bit(uint16_t addr);

    uint8_t memoperation(uint16_t addr, int8_t value);
    void mcbHandleBanking(uint16_t addr, uint8_t value);
    void memoryInit();

    uint16_t pop16(uint16_t &stack);
    void push16(uint16_t &stack, uint16_t value);
};
