cmake_minimum_required(VERSION 3.16)
project(perm)

set(CMAKE_CXX_STANDARD 17)

add_library(perm permutation.cpp permutation.h)
add_executable(test test.cpp permutation.cpp)