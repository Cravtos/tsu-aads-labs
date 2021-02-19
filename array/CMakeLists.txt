cmake_minimum_required(VERSION 3.17)
project(array)

set(CMAKE_CXX_STANDARD 14)

add_library(array array.cpp array.h test.cpp)
add_executable(test test.cpp array.cpp)