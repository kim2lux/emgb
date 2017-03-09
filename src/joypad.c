#include "joypad.h"
#include "GB.h"

void keyDown(struct s_gb * gb_s)
{
	gb_s->gb_cpu.stopCpu = 0;
	switch (gb_s->gb_gpu.event.key.keysym.sym)
	{
		gb_s->gb_cpu.stopCpu = 0;
	case SDLK_ESCAPE:
		gb_s->running = 0;
		break;
	case SDLK_w:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_key &= ~(1 << 3);
		break;
	case SDLK_x:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_key &= ~(1 << 2);
		break;
	case SDLK_c:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_key &= ~(1 << 1);
		break;
	case SDLK_v:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_key &= ~(1 << 0);
		break;
	case SDLK_DOWN:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_dir &= ~(1 << 3);
		break;
	case SDLK_UP:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_dir &= ~(1 << 2);
		break;
	case SDLK_LEFT:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_dir &= ~(1 << 1);
		break;
	case SDLK_RIGHT:
		gb_s->gb_interrupts.interFlag |= INT_JOYPAD;
		gb_s->gb_pad.button_dir &= ~(1 << 0);
		break;
	}
	return;
}

void keyUp(struct s_gb * gb_s)
{
	switch (gb_s->gb_gpu.event.key.keysym.sym)
	{
	case SDLK_w:
		gb_s->gb_pad.button_key |= (1 << 3);
		break;
	case SDLK_x:
		gb_s->gb_pad.button_key |= (1 << 2);
		break;
	case SDLK_c:
		gb_s->gb_pad.button_key |= (1 << 1);
		break;
	case SDLK_v:
		gb_s->gb_pad.button_key |= (1 << 0);
		break;
	case SDLK_DOWN:
		gb_s->gb_pad.button_dir |= (1 << 3);
		break;
	case SDLK_UP:
		gb_s->gb_pad.button_dir |= (1 << 2);
		break;
	case SDLK_LEFT:
		gb_s->gb_pad.button_dir |= (1 << 1);
		break;
	case SDLK_RIGHT:
		gb_s->gb_pad.button_dir |= (1 << 0);
		break;
	}
}

void handleEvent(struct s_gb * gb_s)
{
	if (SDL_PollEvent(&(gb_s->gb_gpu.event)) != 0)
	{
		switch (gb_s->gb_gpu.event.type) {
		case SDL_QUIT: {
			printf("Quit requested, quitting.\n");
			exit(0);
		}
		case SDL_KEYDOWN:
			keyDown(gb_s);
			break;
		case SDL_KEYUP:
			keyUp(gb_s);
			break;
		}
	}
}