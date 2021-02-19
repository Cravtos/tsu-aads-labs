cmake_minimum_required(VERSION 3.17)
project(string)

set(CMAKE_CXX_STANDARD 14)

add_library(string library.cpp library.h)
add_executable(main main.cpp library.cpp)