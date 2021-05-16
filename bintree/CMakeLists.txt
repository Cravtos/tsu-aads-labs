cmake_minimum_required(VERSION 3.19)
project(bintree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(bst src/bintree.cpp)

add_executable(test tests/test.cpp)
target_link_libraries(test bst)