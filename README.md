Gameboy emulator (C++14 project, rendering through SDL)

# GameBoy emulator status
- CPU instructions are now implemented
- Interrupts are working fine (Joypad, LCDC, H-BLANK, TIMER)
- All Blarggs test are passing except the DAA instruction: https://gbdev.gg8.se/wiki/articles/Test_ROMs
- Rom and Ram Banking are still work in progress. I have been able to validate Tetris and Super Mario land launching. 
- Do not expect to be able to play any other game (Mario sprites rendering is still buggy)

# Images

* Mario

![Mario - Screenshot1](screenshots/mario1.png?raw=true) ![Mario - Screenshot2](screenshots/mario2.png?raw=true) 

* Tetris

![Tetris - Screenshot1](screenshots/tetris1.png?raw=true) ![Tetris - Screenshot2](screenshots/tetris2.png?raw=true) 

* Dr Mario

![Dr Mario - Screenshot1](screenshots/drmario1.png?raw=true) ![Dr Mario - Screenshot2](screenshots/drmario2.png?raw=true) 

* Soccer

![Soccer - Screenshot1](screenshots/soccer1.png?raw=true) ![Soccer - Screenshot2](screenshots/soccer2.png?raw=true) 

# Todo

* Improve Rom Banking and support MCB2 MCB3 Cartridge type
* Debug refactoring and class definition in order to ease development
  - setCpu setMemory shared pointer to be reworked asap
* Implement CB instructions

# How to Build
emgb is only able to build on unix Like system for now

** Linux **

* git submodule update in order to retrieve gtest framework.
* Make sure to add the CXX 14 support within gtest CMakeList.txt
    - set(CMAKE_CXX_STANDARD 14)
    - set(CMAKE_CXX_STANDARD_REQUIRED ON)
* Use the cmake & make sure you installed SDL2 & Opengl glu library
* cmake .. && make -j8; ./gameboy rom

