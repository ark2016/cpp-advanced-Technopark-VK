cmake_minimum_required(VERSION 3.10)
project(dz3)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost 1.83 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add executable
add_executable(dz3 main.cpp)

# Link libraries
target_link_libraries(dz3 
    PRIVATE
    nlohmann_json::nlohmann_json
    Boost::boost
)

# Include directories
target_include_directories(dz3 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
)

# Add compiler flags
target_compile_options(dz3 
    PRIVATE 
    -Wall 
    -Wextra
    -pedantic
)
