#pragma once
#include <stdint.h>
#include <SDL.h>
#include "cpu.hpp"

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
	void keyUp(SDL_Event &event, Z80Cpu &cpu);
	void handleEvent(SDL_Event &event, Z80Cpu &cpu);
	uint8_t	key_; // = 0xff;
private:
	uint8_t	keyButton_; // = 0x0f;
	uint8_t	dirButton_; // = 0x0f;
};
