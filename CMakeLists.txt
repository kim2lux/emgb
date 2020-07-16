cmake_minimum_required(VERSION 2.8)
set(CMAKE_PROJECT_VERSION 0.0.1)
cmake_policy(SET CMP0048 NEW)
project(gameboy VERSION 1.1.0)

set(EXEC_GB_NAME gameboy)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ./include)


add_subdirectory(lib/googletest)
add_subdirectory(test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/gpu.cpp
    src/rom.cpp
    src/memory.cpp
    src/joypad.cpp
    src/cb.cpp
    "src/cpu/and.cpp"
    src/cpu/ld.cpp
    src/cpu/cpu.cpp
    src/cpu/inc.cpp
    src/cpu/rotate.cpp
    src/cpu/dec.cpp
    src/cpu/add.cpp
    src/cpu/sbc.cpp
    src/cpu/sub.cpp
    src/cpu/adc.cpp
    src/cpu/jmp.cpp
    src/cpu/stack.cpp
    src/cpu/ctrl.cpp
    src/cpu/rst.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_sdl.cpp
    src/debug.cpp)
set_source_files_properties(src/cpu.cpp PROPERTIES LANGUAGE CXX)
set_source_files_properties(src/rom.cpp PROPERTIES LANGUAGE CXX)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wpedantic -Wextra")
endif(UNIX)

add_executable(${EXEC_GB_NAME} ${SOURCE_FILES})

if(WIN32
   OR MSYS
   OR MINGW)
  target_link_libraries(
    ${EXEC_GB_NAME}
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
  target_link_libraries(${EXEC_GB_NAME} ${SDL2_LIBRARY} -pthread -lglut -lGL -lGLU -lGLEW )
endif(UNIX)
