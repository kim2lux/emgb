#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include <thread>
#include <iostream>

int IMGUI_debugger(Z80Cpu &cpu);

int main(int ac, char **av)
{
    if (ac == 2)
    {
        Cartridge cart;
        Joypad joypad;
        cart.initRom(av[1]);
        Memory memory(cart, joypad);
        Z80Cpu cpu(memory);
        IMGUI_debugger(cpu);
    }
    else {
        std::cout << "./gameboy {rom name}" << std::endl;
    }
}
