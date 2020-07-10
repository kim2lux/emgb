#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "cpu.hpp"
#include <iostream>
#include "joypad.hpp"
#include "gpu.hpp"

int IMGUI_debugger(Z80Cpu &cpu)
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
	SDL_Window *window = SDL_CreateWindow("ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

	Gpu gpu(cpu);

	while (done == false)
	{
		debug = false;
		if (cpu.regs_.pc == 0xdef8 &&  cpu.regs_.af == 0x1280 && cpu.getMemory().read8bit(cpu.regs_.pc) == 0xc2)
		    start = 1;
		while (debug == true && start == 1)
		{
			ImGui_ImplSdl_NewFrame(window);
			{
				ImGui::SetNextWindowPos(ImVec2(20, 20));
				ImGui::Begin("CPU register state");
				ImGui::Text("Gameboy debug window");
				ImGui::Text("a %x f %x af %x", cpu.regs_.a, cpu.regs_.f, cpu.regs_.af);
				ImGui::Text("b %x c %x bc %x", cpu.regs_.b, cpu.regs_.c, cpu.regs_.bc);
				ImGui::Text("d %x e %x de %x", cpu.regs_.d, cpu.regs_.e, cpu.regs_.de);
				ImGui::Text("h %x l %x hl %x", cpu.regs_.h, cpu.regs_.l, cpu.regs_.hl);
				ImGui::Text("pc %x sp %x", cpu.regs_.pc, cpu.regs_.sp);
				ImGui::End();
			}
			{
				ImGui::SetNextWindowSize(ImVec2(300, 90), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(20, 200));
				ImGui::Begin("Flag State");
				ImGui::Text("Z: %s", cpu.isFlagSet(Flag::ZERO_FLAG) ? "1" : "0");
				ImGui::Text("N: %s", cpu.isFlagSet(Flag::NEG_FLAG) ? "1" : "0");
				ImGui::Text("H: %s", cpu.isFlagSet(Flag::HALFC_FLAG) ? "1" : "0");
				ImGui::Text("C: %s", cpu.isFlagSet(Flag::CARRY_FLAG) ? "1" : "0");
				ImGui::End();
			}

						{
				ImGui::SetNextWindowSize(ImVec2(300, 90), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(20, 350));
				ImGui::Begin("Interrupt State");
				ImGui::Text("IE: %x", cpu.getInterrupt().interruptEnable_);
				ImGui::Text("IF: %x", cpu.getInterrupt().interruptRequest_);
				ImGui::Text("ime: %d", cpu.getInterrupt().masterInterrupt_);
				ImGui::End();
			}
			{
				ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiSetCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(200, 20));
				ImGui::Begin("Opcode");
				ImGui::Text("Next opcode %x", cpu.getMemory().read8bit(cpu.regs_.pc));
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
					std::cout << "out dbg" << std::endl;
					debug = false;
					break;
				}
			}
		}

		cpu.getMemory().joypad_.handleEvent(event, cpu);
		cpu.updateTimer();
		exec = cpu.getMemory().read8bit(cpu.regs_.pc++);
		cpu.opcodes_[exec].opFunc();
		if (cpu.fjmp_ == false)
			cpu.regs_.pc += cpu.opcodes_[exec].size;
		cpu.fjmp_ = false;
		cpu.processRequestInterrupt();
		gpu.updateGpu();
		//std::cout << "pc: " << std::hex << (int32_t) (cpu.regs_.pc) << ": " << std::hex << (uint16_t)exec << " -> " << cpu.opcodes_[exec].value << std::endl;
	}
	// Cleanup
	ImGui_ImplSdl_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	return (0);
}
