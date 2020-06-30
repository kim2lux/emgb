cmake_minimum_required(VERSION 2.8)
set(CMAKE_PROJECT_VERSION 0.0.1)
cmake_policy(SET CMP0048 NEW)
project(gameboy VERSION 1.1.0)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ./include)

add_subdirectory(test)
add_subdirectory(lib/googletest)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/rom.cpp
    src/memory.cpp
    src/joypad.cpp
    src/cpu/cpu.cpp)

set_source_files_properties(src/cpu.cpp PROPERTIES LANGUAGE CXX)
set_source_files_properties(src/rom.cpp PROPERTIES LANGUAGE CXX)

if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra")
endif(UNIX)

add_executable(gb ${SOURCE_FILES})

if(WIN32
   OR MSYS
   OR MINGW)
  target_link_libraries(
    gb
    -lmingw32
    -lSDL2main
    -lSDL2
    opengl32
    glu32
    m
    dinput8
    dxguid
    dxerr8
    user32
    gdi32
    winmm
    imm32
    ole32
    oleaut32
    shell32
    version
    uuid)
endif(
  WIN32
  OR MSYS
  OR MINGW)

if(UNIX)
  target_link_libraries(gb ${SDL2_LIBRARY})
endif(UNIX)
