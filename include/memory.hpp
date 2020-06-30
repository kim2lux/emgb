#pragma once

#include "rom.hpp"

class Memory
{
public:
    Memory(Cartridge &cart) : cart_(cart) {
		
	}

	uint8_t sram_[0x2000];
	uint8_t vram_[0x2000];
	uint8_t ram_[0x2000];
	uint8_t oam_[0x00FF];
	uint8_t io_ports_[0x80];
	uint8_t hram_[0x80];
	Cartridge & cart_;

	int write8bit(uint16_t addr, uint8_t value);
	uint8_t memoperation(uint16_t addr, signed char value);
	void mcbHandleBanking(uint16_t addr, uint8_t value);
	uint8_t read8bit(uint16_t addr);
	uint16_t read16bit(uint16_t addr);
	void write16bitToAddr(uint16_t addr, uint16_t value);
	void call_addr_16();
	void memoryInit();
};
