cmake_minimum_required(VERSION 3.17)
project(bignum)

set(CMAKE_CXX_STANDARD 14)

add_library(bignum bignum.cpp bignum.h)
add_executable(test test.cpp bignum.cpp)