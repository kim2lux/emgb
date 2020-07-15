#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "cpu.hpp"
#include <iostream>
#include "joypad.hpp"
#include "gpu.hpp"
#include <chrono>
#include <thread>
#include <memory>

const float DELAY_TIME = 1000.0f / 59.73f;

const int MAX_CYCLES = 70224;
const float FPS = 59.73f;
int IMGUI_debugger(char *rompath)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	int res = SDL_GetCurrentDisplayMode(0, &current);
	if (res != 0)
		SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());
	SDL_Window *window = SDL_CreateWindow("Debug", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	// Setup ImGui binding
	ImGui_ImplSdl_Init(window);
	ImVec4 clear_color = ImColor(114, 144, 154);

	// Main loop
	bool done = false;
	uint8_t exec;
	SDL_Event event;
	bool debug = true;
	uint8_t start = 0;


	Cartridge cart;
	Joypad joypad;
	cart.initRom(rompath);
	Memory memory(cart, joypad);
	std::shared_ptr<Z80Cpu> cpu = std::make_shared<Z80Cpu>(memory);
	std::shared_ptr<Gpu> gpu = std::make_shared<Gpu>(*cpu);
	cpu->getMemory().setGpu(gpu);
	cpu->getMemory().setCpu(cpu);


	std::chrono::time_point<std::chrono::high_resolution_clock> cur, previous;
	previous = std::chrono::high_resolution_clock::now();
	while (done == false)
	{
		cur = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (cur - previous);
		previous = cur;
		debug = true;
		if (cpu->regs_.pc == 0x20f)
		    start = 1;
		while (debug == false && start == 1)
		{
			ImGui_ImplSdl_NewFrame(window);
			{
				ImGui::Begin("CPU register state");
				ImGui::Text("Gameboy debug window");
				ImGui::Text("a %x f %x af %x", cpu->regs_.a, cpu->regs_.f, cpu->regs_.af);
				ImGui::Text("b %x c %x bc %x", cpu->regs_.b, cpu->regs_.c, cpu->regs_.bc);
				ImGui::Text("d %x e %x de %x", cpu->regs_.d, cpu->regs_.e, cpu->regs_.de);
				ImGui::Text("h %x l %x hl %x", cpu->regs_.h, cpu->regs_.l, cpu->regs_.hl);
				ImGui::Text("pc %x sp %x", cpu->regs_.pc, cpu->regs_.sp);
				ImGui::End();
			}
				ImGui::Begin("Stack Pointer");
				ImGui::Text("SP");
				for (uint16_t value = 0xcfff; value >= 0xcfe9; value-=2) {
				    ImGui::Text("%x -> %x", value, cpu->getMemory().read16bit(value));
				}
				ImGui::End();

			{
				gpu->updateGpuRegister();
				ImGui::Begin("LCD Ctrl");
				ImGui::Text("LCD Enable: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::LCD_DISPLAY) ? 1 : 0);
				ImGui::Text("Window Tile Map: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::WIN_TILE_MAP_DISPLAY) ? window_tile_display_map_select_high : window_tile_display_map_select_low);
				ImGui::Text("Window Enable: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::WIN_DISPLAY) ? 1 : 0);
				ImGui::Text("Tile Addr: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::BG_WIN_TILE_DATA_SELECT) ? tile_data_addr_low : tile_data_addr_high);
				ImGui::Text("Bg Tile Map: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::BG_TILE_MAP_DISPLAY) ? bg_tile_map_display_select_high : bg_tile_map_display_select_low);
				ImGui::Text("Sprite Size: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::OBJ_SIZE) ? 16 : 8);
				ImGui::Text("Sprite Enable: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::OBJ_DISPLAY) ? 1 : 0);
				ImGui::Text("BG/WIN Display: %x", isBitSet(gpu->lcdCtrl_, LcdCTrl::BG_WIN_PRIORITY) ? 1 : 0);

				ImGui::End();
			}
			{
				ImGui::Begin("GPU Rendering");
				ImGui::Text("Scanline: %d", gpu->getScanline());
				ImGui::Text("GPU Mode: %d", gpu->getGpuMode());
				ImGui::Text("Scroll X: %x", gpu->scX_);
				ImGui::Text("Scroll Y: %x", gpu->scY_);
				ImGui::Text("Window X: %x", gpu->winX_);
				ImGui::Text("Window Y: %x", gpu->winY_);
				ImGui::End();
			}
			{
				ImGui::Begin("Flag Register");
				ImGui::Text("Z: %s", cpu->isFlagSet(Flag::ZERO_FLAG) ? "1" : "0");
				ImGui::Text("N: %s", cpu->isFlagSet(Flag::NEG_FLAG) ? "1" : "0");
				ImGui::Text("H: %s", cpu->isFlagSet(Flag::HALFC_FLAG) ? "1" : "0");
				ImGui::Text("C: %s", cpu->isFlagSet(Flag::CARRY_FLAG) ? "1" : "0");
				ImGui::End();
			}
			{
				ImGui::Begin("Timer");
				ImGui::Text("Timer Counter TIMA: %x", cpu->timer_.tima_);
				ImGui::Text("Timer Modulo TMA: %x", cpu->timer_.tma_);
				ImGui::Text("Timer Control TAC: %x", cpu->timer_.tac_);
				ImGui::End();
			}
			{
				ImGui::Begin("Interrupt State");
				ImGui::Text("IE: %x", cpu->getInterrupt().interruptEnable_);
				ImGui::Text("IF: %x", cpu->getInterrupt().interruptRequest_);
				ImGui::Text("ime: %d", cpu->getInterrupt().masterInterrupt_);
				ImGui::End();
			}
			{
				ImGui::Begin("Opcode");
				ImGui::Text("Next opcode %x", cpu->getMemory().read8bit(cpu->regs_.pc));
				ImGui::End();
			}

			// Rendering
			glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui::Render();
			SDL_GL_SwapWindow(window);
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

				case SDL_KEYDOWN:
					debug = false;
					break;
				}
			}
		}

		uint32_t prevTickCount = 0;
		cpu->getMemory().joypad_.handleEvent(event, *cpu);
		gpu->gpuCycle_ = 0;
		cpu->tickCount_ = 0;
		while (cpu->tickCount_ < 70224)
		{
			
			prevTickCount = cpu->tickCount_;
			if (cpu->halt_  == false) {
				exec = cpu->getMemory().read8bit(cpu->regs_.pc++);
				std::cout << "pc: " << std::hex << (int32_t) (cpu->regs_.pc - 1) << ": " << std::hex << (uint16_t)exec << " -> " << cpu->opcodes_[exec].value << std::endl;
				cpu->opcodes_[exec].opFunc();
				if (cpu->fjmp_ == false)
					cpu->regs_.pc += cpu->opcodes_[exec].size;
			}
			else {
				cpu->tickCount_ += 4;
			}
			cpu->updateTimer(cpu->tickCount_ - prevTickCount);
			cpu->fjmp_ = false;
			gpu->updateGpu(cpu->tickCount_ - prevTickCount);
			cpu->processRequestInterrupt();
		}
	    gpu->render();

		if (elapsed.count() < DELAY_TIME)
		{
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));
		}
		else
		{
			printf("overpassed by %f\n", elapsed.count() - DELAY_TIME);
		}
	}
	ImGui_ImplSdl_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	return (0);
}
