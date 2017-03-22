Another Gameboy emulator (C project, rendering through SDL)


# Z80 CPU
- Most of CPU instructions are now implemented
- CB opcode is still missing, currently using https://github.com/CTurt/Cinoop/blob/master/source/cb.c

# TODO 
cpu instructions : 
- DAA
- PUSH/POP (AF)
- RAM Writing error (vram not loaded properly for mario)

gpu:
- Background rendering is still messy, need optimization and code clearing
- Sprite rendering is basic => need to implement sprite tile options 

# How to

build:
I could test on windows and Debian/Ubuntu

Windows: 
I could use both MSYS & Visual Studio (create new project & link file for now)
For MSYS build you can directly build thanks to cmake

Linux: 
Use the cmake & make sure you installed SDL2 library


