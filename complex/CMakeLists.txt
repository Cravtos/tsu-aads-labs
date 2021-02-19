cmake_minimum_required(VERSION 3.16)
project(complex)

set(CMAKE_CXX_STANDARD 17)

add_library(complex complex.cpp complex.hpp)
add_executable(test test.cpp complex.cpp complex.hpp)