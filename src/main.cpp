#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include <thread>
#include <iostream>

int IMGUI_debugger(char *, char *);

int main(int ac, char **av)
{
    if (ac >= 2)
    {
        IMGUI_debugger(av[1], av[2]);
    }
    else {
        std::cout << "./gameboy {rom name}" << std::endl;
    }
}
