#include "rom.hpp"
#include "memory.hpp"
#include "cpu.hpp"

int main(int ac, char **av)
{
    Cartridge cart;
    cart.initRom(av[1]);
    Memory memory(cart);
    Z80Cpu cpu(memory);
    uint8_t exec;

    while (cpu.running)
    {
        exec = memory.read8bit(cpu.regs_.pc++);
        cpu.opcodes_[exec].opFunc();
        if (cpu.jmp == 0)
            cpu.regs_.pc += cpu.opcodes_[exec].size;
        cpu.jmp = 0;
        //updateGpu(s_gb);
        //doInterupt(s_gb);
        //updateTimer(s_gb);
    }
}
