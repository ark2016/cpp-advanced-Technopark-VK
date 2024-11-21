cmake_minimum_required(VERSION 3.28)
project(dz1)

set(CMAKE_CXX_STANDARD 20)

add_executable(dz1 main.cpp intrusive_list.hpp lru.hpp)

# Enable testing
enable_testing()

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executables
add_executable(test_intrusive_list test_intrusive_list.cpp intrusive_list.hpp)
target_link_libraries(test_intrusive_list ${GTEST_LIBRARIES} pthread)

add_executable(test_lru test_lru.cpp lru.hpp intrusive_list.hpp)
target_link_libraries(test_lru ${GTEST_LIBRARIES} pthread)

# Add tests
add_test(NAME test_intrusive_list COMMAND test_intrusive_list)
add_test(NAME test_lru COMMAND test_lru)

# Enable coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

