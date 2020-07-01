set(BINARY test-cpu)
file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false
  *.h *.cpp)
set(SOURCES ${TEST_SOURCES} ${PROJECT_SOURCE_DIR}/src/cpu/cpu.cpp
            ${PROJECT_SOURCE_DIR}/src/cpu/ld.cpp
            ${PROJECT_SOURCE_DIR}/src/cpu/inc.cpp
            ${PROJECT_SOURCE_DIR}/src/memory.cpp
            ${PROJECT_SOURCE_DIR}/src/rom.cpp)
add_executable(${BINARY} ${SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC gtest ${SDL2_LIBRARY})
