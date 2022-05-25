#include "../include/bignum.h"

#include <iostream>

int main() {
    BigNum number(base_t(23291)); // is number
    std::cout << number << std::endl;
    std::cout << (number.fermat_prime_test(500) == 1
        ? "Is prime acc. to Fermat"
        : "Is not prime acc. to Fermat")
        << std::endl;
    std::cout << (number.solovay_strassen_prime_test(15) == 1
        ? "Is prime acc. to Solovay Strassen"
        : "Is not prime acc. Solovay Strassen")
        << std::endl;

    number = ext_base_t(15208);
    std::cout << number << std::endl;
    std::cout << (number.fermat_prime_test(10) == 1
                  ? "Is prime acc. to Fermat"
                  : "Is not prime acc. to Fermat")
              << std::endl;
    std::cout << (number.solovay_strassen_prime_test(10) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;

    number = ext_base_t(15209);
    std::cout << number << std::endl;
    std::cout << (number.fermat_prime_test(10) == 1
                  ? "Is prime acc. to Fermat"
                  : "Is not prime acc. to Fermat")
              << std::endl;
    std::cout << (number.solovay_strassen_prime_test(10) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;

    number = ext_base_t(3);
    std::cout << number << std::endl;
    std::cout << (number.fermat_prime_test(10) == 1
                  ? "Is prime acc. to Fermat"
                  : "Is not prime acc. to Fermat")
              << std::endl;
    std::cout << (number.solovay_strassen_prime_test(10) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;

    number = ext_base_t(2);
    std::cout << number << std::endl;
    std::cout << (number.fermat_prime_test(10) == 1
                  ? "Is prime acc. to Fermat"
                  : "Is not prime acc. to Fermat")
              << std::endl;
    std::cout << (number.solovay_strassen_prime_test(10) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;
}