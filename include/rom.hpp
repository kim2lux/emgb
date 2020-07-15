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
	uint8_t read8bit(uint16_t addr) override {
		return (rom_[addr]);
	}
	void write8bit(uint16_t addr, uint8_t value) override {
        rom_[addr] = value;
	}
};

class Mcb1 : public  CartridgeTypeInterface {
	uint8_t read8bit(uint16_t addr) {
		return (rom_[addr]);
	}
	void write8bit(uint16_t addr, uint8_t value) {
        rom_[addr] = value;
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
