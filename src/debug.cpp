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

class Debug
{
public:
    SDL_Window *window_;
    SDL_GLContext glContext_;
    ImVec4 clearColor_;
    SDL_Event event_;

    ~Debug()
    {
        ImGui_ImplSdl_Shutdown();
        SDL_GL_DeleteContext(glContext_);
        SDL_DestroyWindow(window_);
    }

    Debug()
    {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        window_ = SDL_CreateWindow("Debug", 30, 30, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        glContext_ = SDL_GL_CreateContext(window_);

        // Setup ImGui binding
        ImGui_ImplSdl_Init(window_);
        clearColor_ = ImColor(60, 60, 60);
    }
    void renderDebug(const std::shared_ptr<Z80Cpu> &cpu, const std::shared_ptr<Gpu> &gpu)
    {
        ImGui_ImplSdl_NewFrame(window_);
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
        for (uint16_t value = 0xcfff; value >= 0xcfe9; value -= 2)
        {
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
    }

    void Rendering() {
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clearColor_.x, clearColor_.y, clearColor_.z, clearColor_.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window_);
    }
};

void load(std::shared_ptr<Z80Cpu> cpu, char *saveState)
{
    std::ifstream input(saveState, std::ios::binary | std::ios::in);

    input.seekg(0, std::ios::end);
    auto fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    assert(fileSize != 0);
    std::vector<uint8_t> buffer(fileSize);
    buffer.resize(fileSize);
    input.read((char*)buffer.data(), fileSize);
    cpu->getMemory().load(buffer);
    uint32_t pos = cpu->getMemory().cart_.cartridgeType_->load(buffer);
    cpu->load(buffer, pos);
    input.close();
}

int IMGUI_debugger(char *rompath, char *saveState = nullptr)
{
    bool done = false;
    uint8_t exec;
    SDL_Event event;

    Cartridge cart;
    Joypad joypad;
    cart.initRom(rompath);
    Memory memory(cart, joypad);
    joypad.setRomPath(rompath);
    std::shared_ptr<Z80Cpu> cpu = std::make_shared<Z80Cpu>(memory);
    std::shared_ptr<Gpu> gpu = std::make_shared<Gpu>(*cpu);
    cpu->getMemory().setGpu(gpu);
    cpu->getMemory().setCpu(cpu);
    joypad.setGpu(gpu);

    Debug dbg;
    if (saveState != nullptr) {
        load(cpu, saveState);
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> cur, previous;
    previous = std::chrono::high_resolution_clock::now();
    while (done == false)
    {
        dbg.renderDebug(cpu, gpu);
        cur = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(cur - previous);
        previous = cur;
        uint32_t prevTickCount = 0;
        cpu->getMemory().joypad_.handleEvent(event, *cpu);
        gpu->gpuCycle_ = 0;
        cpu->tickCount_ = 0;
        while (cpu->tickCount_ < 70224)
        {
            prevTickCount = cpu->tickCount_;
            if (cpu->halt_ == false)
            {
                exec = cpu->getMemory().read8bit(cpu->regs_.pc++);
                //std::cout << "pc: " << std::hex << (int32_t) (cpu->regs_.pc - 1) << ": " << std::hex << (uint16_t)exec << " -> " << cpu->opcodes_[exec].value << std::endl;
                cpu->opcodes_[exec].opFunc();
                if (cpu->fjmp_ == false)
                    cpu->regs_.pc += cpu->opcodes_[exec].size;
            }
            else
            {
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
            dbg.Rendering();
        }
    }
    return (0);
}
