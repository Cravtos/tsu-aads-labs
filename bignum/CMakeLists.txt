cmake_minimum_required(VERSION 3.16.3)
project(bignum)

set(CMAKE_CXX_STANDARD 14)

add_library(bignum src/arithmetic.cpp src/constructors.cpp src/compare.cpp include/bignum.h src/io.cpp src/helpers.cpp)
add_library(fraction src/fraction.cpp)
link_libraries(bignum fraction)

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

add_executable(test_division tests/test_division.cpp)
target_link_libraries(test_division bignum)

add_executable(test_fast_sq tests/test_fast_sq.cpp)
target_link_libraries(test_fast_sq bignum)

add_executable(test_pow tests/test_pow.cpp)
target_link_libraries(test_pow bignum)

add_executable(test_prime tests/test_prime.cpp)
target_link_libraries(test_prime bignum)