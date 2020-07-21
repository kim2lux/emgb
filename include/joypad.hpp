#pragma once
#include <stdint.h>
#include <SDL.h>
#include "cpu.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include "cpu.hpp"
#include "memory.hpp"

class Z80Cpu;
class Joypad
{
public:
	Joypad()
	{
		key_ = 0xff;
		keyButton_ = 0x0f;
		dirButton_ = 0x0f;
	}
	uint8_t padState();
	void keyDown(SDL_Event &event, Z80Cpu &cpu);
	void keyUp(SDL_Event &event);
	void handleEvent(SDL_Event &event, Z80Cpu &cpu);
	uint8_t	key_; // = 0xff;

	void save(const Z80Cpu &cpu);
private:
	uint8_t	keyButton_; // = 0x0f;
	uint8_t	dirButton_; // = 0x0f;
};
