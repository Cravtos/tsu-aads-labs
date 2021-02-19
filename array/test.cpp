#include "array.h"

#include <iostream>
#include <chrono>
#include <unistd.h>

int main() {
    std::cout << "Test start" << std::endl;

    const size_t N = 1e5;
    Array arr(N, 0, N);

    std::cout << "Amount of elements to sort: " << N << std::endl;

    // SHELL SORT
    std::cout << std::endl << "SHELL SORT" << std::endl;
    auto arr_cpy = arr;
    auto begin = std::chrono::high_resolution_clock::now();
    arr_cpy.shell_sort(INCR);
    auto end = std::chrono::high_resolution_clock::now();

    if (!arr_cpy.is_incr() && arr_cpy == arr) {
        std::cout << "Err! Shellsort is not working!" << std::endl;
        return 1;
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "time taken: " << duration << "ms" << std::endl;

//    sleep(3);

    // HEAP SORT
    std::cout << std::endl << "HEAP SORT" << std::endl;
    arr_cpy = arr;
    begin = std::chrono::high_resolution_clock::now();
    arr_cpy.heap_sort(INCR);
    end = std::chrono::high_resolution_clock::now();

    if (!arr_cpy.is_incr() && arr_cpy == arr) {
        std::cout << "Err! Heapsort is not working!" << std::endl;
        return 1;
    }

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "time taken: " << duration << "ms" << std::endl;

//    sleep(3);

    // QUICK SORT
    std::cout << std::endl << "QUICK SORT" << std::endl;
    arr_cpy = arr;
    begin = std::chrono::high_resolution_clock::now();
    arr_cpy.quick_sort(INCR);
    end = std::chrono::high_resolution_clock::now();

    if (!arr_cpy.is_incr() && arr_cpy == arr) {
        std::cout << "Err! Quicksort is not working!" << std::endl;
        return 1;
    }

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "time taken: " << duration << "ms" << std::endl;


//    sleep(3);

//    // SHAKER SORT
//    std::cout << std::endl << "SHAKER SORT" << std::endl;
//    arr_cpy = arr;
//    begin = std::chrono::high_resolution_clock::now();
//    arr_cpy.shaker_sort(INCR);
//    end = std::chrono::high_resolution_clock::now();
//
//    if (!arr_cpy.is_incr() && arr_cpy == arr) {
//        std::cout << "Err! Shakersort is not working!" << std::endl;
//        return 1;
//    }
//
//    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
//    std::cout << "time taken: " << duration << "ms" << std::endl;
//
//    return 0;
}
