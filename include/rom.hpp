#pragma once

#include <stdint.h>
#include <array>
#include <algorithm>
#include "SDL.h"
#include <assert.h>

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

class Cartridge
{
private:
	struct s_romHeader romheader_;

public:
	std::array<uint8_t, 1000000> rom_;
	uint32_t size_;

	int initRom(char *filename);
	int loadRom(char *romfile);
	void freeRom();
	void loadHeader();
	void displayHeader();
	s_romHeader &getHeader();
};
