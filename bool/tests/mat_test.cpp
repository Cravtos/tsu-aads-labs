#include "../include/bool_mat.h"

int main()
{
    BoolMat bm1(5, 5);
    std::cout << "Constructor from two integers: " << std::endl;
    std::cout << bm1 << std::endl;

    auto bm2(bm1);
    bm2.one_diag();
    std::cout << "one_diag() and copy constructor: " << std::endl;
    std::cout << bm2 << std::endl;

    std::cout << "null_diag(): " << std::endl;
    bm2.null_diag();
    std::cout << bm2 << std::endl;

    bm2.one_diag();
    bm1 = bm2;
    bm1[3].set_1(4);
    bm1.empty_row(2);

    std::cout << "operator= + some operations : " << std::endl;
    std::cout << bm1 << std::endl;

    int* ones = bm1.cols_ones();

    std::cout << "cols_ones(): " << std::endl;
    for (ssize_t i = 0; i < 5; i++) {
        std::cout << ones[i] << " ";
    }
    std::cout << std::endl;

    delete[] ones;

    return 0;
}