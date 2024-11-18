cmake_minimum_required(VERSION 3.28)
project(dz1)

set(CMAKE_CXX_STANDARD 20)

add_executable(dz1 main.cpp
#        test_intrusive_list.cpp
#        test_lru.cpp
        intrusive_list.hpp
        lru.hpp
#        hashtable.hpp
#        test_hashtable.cpp
)
