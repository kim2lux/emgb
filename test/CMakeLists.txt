set(BINARY test-cpu)
file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false
  *.h *.cpp)
set(SOURCES
    ${TEST_SOURCES}
    ${PROJECT_SOURCE_DIR}/src/cpu/cpu.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/ld.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/add.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/inc.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/dec.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/jmp.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/adc.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/sbc.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/sub.cpp
    ${PROJECT_SOURCE_DIR}/src/cpu/stack.cpp
    "${PROJECT_SOURCE_DIR}/src/cpu/and.cpp"
    ${PROJECT_SOURCE_DIR}/src/cpu/rotate.cpp
    ${PROJECT_SOURCE_DIR}/src/memory.cpp
    ${PROJECT_SOURCE_DIR}/src/rom.cpp)
enable_testing()

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} PUBLIC gtest ${SDL2_LIBRARY})

add_test(NAME ${BINARY} COMMAND ${BINARY})