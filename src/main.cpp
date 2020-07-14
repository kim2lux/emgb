#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include <thread>
#include <iostream>

int IMGUI_debugger(char *);

int main(int ac, char **av)
{
    if (ac == 2)
    {
        IMGUI_debugger(av[1]);
    }
    else {
        std::cout << "./gameboy {rom name}" << std::endl;
    }
}
