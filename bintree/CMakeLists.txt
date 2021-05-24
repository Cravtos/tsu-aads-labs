cmake_minimum_required(VERSION 3.19)
project(bintree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(node src/node.cpp)

add_library(bst src/bst.cpp)
link_libraries(bst node)

add_library(avl src/avl.cpp)
link_libraries(avl bst)

add_executable(test_bst tests/test_bst.cpp)
add_executable(test_avl tests/test_avl.cpp)

target_link_libraries(test_bst bst)
target_link_libraries(test_avl avl)