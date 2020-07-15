#pragma once

#include <stdint.h>
#include <array>
#include <algorithm>
#include "SDL.h"
#include <assert.h>
#include <memory>
#include <vector>

#pragma pack(push)
struct s_romHeader
{
    int entrypoint;
    char nlogo[48];
    char title[16];
    unsigned short manufacturerCode;
    unsigned char cgbFlag;
    unsigned char cartridgeType;
    unsigned char romSize;
    unsigned char ramSize;
    unsigned char destCode; //00 JPN 01 N-JPN
    unsigned char oldLicenseeCode;
    unsigned char gameVersion;
    unsigned char headerCheckSum;
    unsigned short glbCheckSum;
};

#pragma pack(pop)

static constexpr uint32_t header_start = 0x100;
static constexpr uint32_t header_end = 0x014F;

class CartridgeTypeInterface {
public:
    virtual uint8_t read8bit(uint16_t addr) = 0;
    virtual void write8bit(uint16_t addr, uint8_t value) = 0;

    CartridgeTypeInterface(){

    }
    virtual ~CartridgeTypeInterface() {}

    std::array<uint8_t, 10000000> rom_;
};

class Mcb0 : public  CartridgeTypeInterface {
public:
    uint8_t read8bit(uint16_t addr) override {
        return (rom_[addr]);
    }
    void write8bit(uint16_t addr, uint8_t value) override {
        rom_[addr] = value;
    }
};

static constexpr uint16_t ram_enable_end = 0x1fff;
static constexpr uint16_t ROM_BANK_SIZE = 0x4000;
static constexpr uint16_t RAM_BANK_SIZE = 0x2000;

enum RomSelect : uint8_t {
    ROM = 0,
    RAM = 1
};

class Mcb1 : public  CartridgeTypeInterface {
public:
    uint8_t currentRomBank_ ;
    RomSelect mode_;
    bool ramEnable_;
    std::array<uint8_t, RAM_BANK_SIZE * 4> ram_;

    Mcb1() {
        memset(&ram_, 0xff, RAM_BANK_SIZE * 4);
        currentRomBank_ = 1;
        mode_ = RomSelect::ROM;
        ramEnable_ = false;
    }

    uint8_t read8bit(uint16_t addr)
    {
        if (addr <= 0x3FFF)
        {
            return (rom_[addr]);
        }
        else if (addr <= 0x7FFF)
        {
            uint32_t bankaddr = (currentRomBank_ - 1) * ROM_BANK_SIZE;

            return rom_[addr + bankaddr];
        }
        else if (addr >= 0xA000 && addr < 0xC000)
        {
            return ram_[addr - 0xA000];
        }
        assert(1==1);
        return (0);
    }

    void write8bit(uint16_t addr, uint8_t value) {
        if (addr <= ram_enable_end) {
            if ((value & 0x0a) == 0x0a) {
                ramEnable_ = true;
            } else { //should check 0x0a value ?
                ramEnable_ = false;
            }
        }

        else if (addr >= 0x2000 && addr <= 0x3fff) {
            if (mode_ == RomSelect::ROM) {
                uint8_t bankValueTmp = (value & 0x1f);
                currentRomBank_ &= ~(0x1f);
                currentRomBank_ |= bankValueTmp;
                if (currentRomBank_ == 0) {
                    currentRomBank_ = 1;
                }
            }
        }

        else if (addr >= 0x4000 && addr <= 0x5fff) {
            if (mode_ == RomSelect::ROM) {
                uint8_t bankValueTmp = (value & 0x03) << 5;
                currentRomBank_ &= ~(0x60);
                currentRomBank_ |= bankValueTmp;
                if (currentRomBank_ == 0) {
                    currentRomBank_ = 1;
                }
            }
        }
        else if (addr >= 0x6000 && addr <= 0x7fff) {
            if (value == 0) {
                mode_ = RomSelect::ROM;
            }
            else if (value == 1) {
                mode_ = RomSelect::RAM;
            }
        }
        else if (addr >= 0xA000 && addr < 0xC000)
        {
            ram_[addr - 0xA000] = value;
        }
        assert(1==1);
    }
};

class Cartridge
{
private:
    struct s_romHeader romheader_;

public:
    uint32_t size_;


    uint8_t read8bit(uint16_t addr) {
        return (cartridgeType_->read8bit(addr));
    }
    void write8bit(uint16_t addr, uint8_t value) {
        cartridgeType_->write8bit(addr, value);
    }

    int initRom(char *filename);
    int loadRom(char *romfile);

    std::unique_ptr<CartridgeTypeInterface> cartridgeType_;

    void freeRom();
    int loadHeader(char *romfile);
    void loadcartridgeType() {
        switch (romheader_.cartridgeType) {
            case 0: cartridgeType_ = std::make_unique<Mcb0>(); break;
            case 1: cartridgeType_ = std::make_unique<Mcb1>(); break;
        }
    }
    void displayHeader();
    s_romHeader &getHeader();
};
