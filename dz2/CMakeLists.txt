cmake_minimum_required(VERSION 3.28)
project(dz2)

set(CMAKE_CXX_STANDARD 20)

# Add the main application
add_executable(dz2 main.cpp units/unit.cpp units/crew_members.cpp units/passengers.cpp units/plane.cpp)

# Add tests
add_executable(test_crew_members tests/crew_members.cpp units/unit.cpp units/crew_members.cpp)
add_executable(test_passengers tests/passengers.cpp units/unit.cpp units/passengers.cpp)
add_executable(test_plane tests/plane.cpp units/unit.cpp units/crew_members.cpp units/passengers.cpp units/plane.cpp)

