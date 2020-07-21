#include "joypad.hpp"
#include "cpu.hpp"
#include "utils.h"

uint8_t Joypad::padState()
{
	if ((key_ & 0x20) == 0)
		return (0xc0 | keyButton_ | 0x10);
	else if ((key_ & 0x10) == 0)
		return (0xc0 | dirButton_ | 0x20);
	return (0xf0);
}

void Joypad::keyDown(SDL_Event &event, Z80Cpu &cpu)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_w:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		keyButton_ &= ~(1 << 3);
		break;
	case SDLK_x:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		keyButton_ &= ~(1 << 2);
		break;
	case SDLK_c:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		keyButton_ &= ~(1 << 1);
		break;
	case SDLK_v:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		keyButton_ &= ~(1 << 0);
		break;
	case SDLK_DOWN:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		dirButton_ &= ~(1 << 3);
		break;
	case SDLK_UP:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		dirButton_ &= ~(1 << 2);
		break;
	case SDLK_LEFT:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		dirButton_ &= ~(1 << 1);
		break;
	case SDLK_RIGHT:
		setBit(cpu.getInterrupt().interruptRequest_, InterruptType::JOYPAD);
		dirButton_ &= ~(1 << 0);
		break;
	case SDLK_F5:
	    std::cout << "saving..." << std::endl;
	    save(cpu);
		break;
	}
	return;
}

void Joypad::keyUp(SDL_Event &event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_w:
		keyButton_ |= (1 << 3);
		break;
	case SDLK_x:
		keyButton_ |= (1 << 2);
		break;
	case SDLK_c:
		keyButton_ |= (1 << 1);
		break;
	case SDLK_v:
		keyButton_ |= (1 << 0);
		break;
	case SDLK_DOWN:
		dirButton_ |= (1 << 3);
		break;
	case SDLK_UP:
		dirButton_ |= (1 << 2);
		break;
	case SDLK_LEFT:
		dirButton_ |= (1 << 1);
		break;
	case SDLK_RIGHT:
		dirButton_ |= (1 << 0);
		break;
	}
}

void Joypad::handleEvent(SDL_Event &event, Z80Cpu &cpu)
{
	if (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			printf("see u.\n");
			exit(0);
			break;
		}
		case SDLK_ESCAPE:
		{
			printf("see u.\n");
			exit(0);
			break;
		}
		case SDL_KEYDOWN:
			keyDown(event, cpu);
			break;
		case SDL_KEYUP:
			keyUp(event);
			break;
		}
	}
}

void Joypad::save(const Z80Cpu &cpu)
{
	std::vector<uint8_t> tosave;
	size_t found = rompath_.find_last_of("/\\");
	tosave = std::move(cpu.getMemory().serialize());
	std::vector<uint8_t> cartsave = std::move(cpu.getMemory().cart_.cartridgeType_->serialize());
	std::vector<uint8_t> cpusave = std::move(cpu.serialize());

	std::string filename = rompath_.substr(found+1) + ".save";
	std::cout << "saving in: " << filename << std::endl;
	std::copy(cartsave.begin(), cartsave.end(), std::back_inserter(tosave));
	std::copy(cpusave.begin(), cpusave.end(), std::back_inserter(tosave));
	std::ofstream outputfile(filename, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
	std::copy(tosave.begin(), tosave.end(), std::ostream_iterator<uint8_t>(outputfile));
	outputfile.close();
}
