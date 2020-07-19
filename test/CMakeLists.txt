set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BINARY test-gb)
file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false
  *.h *.cpp)

enable_testing()

add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} PUBLIC gmb gtest ${SDL2_LIBRARY})

add_test(NAME ${BINARY} COMMAND ${BINARY})
