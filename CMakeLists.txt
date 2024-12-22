cmake_minimum_required(VERSION 3.10)
project(boost_fusion_serialize_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(boost_fusion_serialize_example main.cpp)
target_include_directories(boost_fusion_serialize_example PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${nlohmann_json_INCLUDE_DIRS}
)
target_link_libraries(boost_fusion_serialize_example PRIVATE nlohmann_json::nlohmann_json)
