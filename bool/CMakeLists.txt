cmake_minimum_required(VERSION 3.17)
project(bool)

set(CMAKE_CXX_STANDARD 14)

add_library(vec include/bool_vec.h src/bool_vec.cpp)
add_library(mat include/bool_mat.h src/bool_mat.cpp)


add_executable(testvec tests/vec_test.cpp src/bool_vec.cpp)
add_executable(testmat tests/mat_test.cpp src/bool_vec.cpp src/bool_mat.cpp)
add_executable(testtop tests/top_test.cpp src/top_sort.cpp src/bool_vec.cpp src/bool_mat.cpp)