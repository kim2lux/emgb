cmake_minimum_required(VERSION 2.8)
project(gb)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)


include_directories(${SDL2_INCLUDE_DIR}  ${OPENGL_INCLUDE_DIR} ./include)

set(SOURCE_FILES src/main.c src/rom.c src/gb.c src/cpu.c src/gpu.c src/memory.c src/io.c src/cb.c src/interupt.c  src/joypad.c src/timer.c src/background.c src/seeu.c src/init.c)

if (UNIX)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")
endif (UNIX)


add_executable(gb ${SOURCE_FILES})

if (WIN32 OR MSYS OR MINGW )
    target_link_libraries(gb -lmingw32 -lSDL2main -lSDL2  opengl32  glu32 m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 version uuid)
endif (WIN32 OR MSYS OR MINGW )

if (UNIX)  
   target_link_libraries(gb ${SDL2_LIBRARY})
endif (UNIX)
