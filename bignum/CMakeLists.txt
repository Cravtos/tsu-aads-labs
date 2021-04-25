cmake_minimum_required(VERSION 3.17)
project(bignum)

set(CMAKE_CXX_STANDARD 14)

add_library(bignum bignum.cpp bignum.h)

add_executable(test_constructors tests/test_constructors.cpp)
target_link_libraries(test_constructors bignum)

add_executable(test_sum tests/test_sum.cpp)
target_link_libraries(test_sum bignum)

add_executable(test_compare tests/test_compare.cpp)
target_link_libraries(test_compare bignum)

add_executable(test_mult tests/test_mult.cpp)
target_link_libraries(test_mult bignum)

add_executable(test_io tests/test_io.cpp)
target_link_libraries(test_io bignum)