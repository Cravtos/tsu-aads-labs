#include "../include/bignum.h"

#include <iostream>

int main() {
    BigNum strong_prime = gen_strong_prime(2);
    std::cout << "Generated strong prime: " << strong_prime << std::endl;
    std::cout << (strong_prime.solovay_strassen_prime_test(100) == 1
                  ? "Is prime acc. to Solovay Strassen"
                  : "Is not prime acc. Solovay Strassen")
              << std::endl;


    // False positive example
    BigNum number = ext_base_t(565);

    size_t attempt = 0;
    while (number.fermat_prime_test(3) == false) { attempt++; }
    std::cout << "Fermat prime test lied on " << attempt << " attempt" << std::endl;

    attempt = 0;
    while (number.solovay_strassen_prime_test(2) == false) { attempt++; }
    std::cout << "Solovay Strassen prime test lied on " << attempt << " attempt" << std::endl;
    // ---

//    number = base_t(23291); // is prime number
//    std::cout << number << std::endl;
//    std::cout << (number.fermat_prime_test(500) == 1
//        ? "Is prime acc. to Fermat"
//        : "Is not prime acc. to Fermat")
//        << std::endl;
//    std::cout << (number.solovay_strassen_prime_test(15) == 1
//        ? "Is prime acc. to Solovay Strassen"
//        : "Is not prime acc. Solovay Strassen")
//        << std::endl;
//
//    std::cout << (number.fermat_prime_test(10) == 1
//                  ? "Is prime acc. to Fermat"
//                  : "Is not prime acc. to Fermat")
//              << std::endl;
//    std::cout << (number.solovay_strassen_prime_test(10) == 1
//                  ? "Is prime acc. to Solovay Strassen"
//                  : "Is not prime acc. Solovay Strassen")
//              << std::endl;
//
//    number = ext_base_t(15209);
//    std::cout << number << std::endl;
//    std::cout << (number.fermat_prime_test(1) == 1
//                  ? "Is prime acc. to Fermat"
//                  : "Is not prime acc. to Fermat")
//              << std::endl;
//    std::cout << (number.solovay_strassen_prime_test(10) == 1
//                  ? "Is prime acc. to Solovay Strassen"
//                  : "Is not prime acc. Solovay Strassen")
//              << std::endl;
//
//    number = ext_base_t(3);
//    std::cout << number << std::endl;
//    std::cout << (number.fermat_prime_test(10) == 1
//                  ? "Is prime acc. to Fermat"
//                  : "Is not prime acc. to Fermat")
//              << std::endl;
//    std::cout << (number.solovay_strassen_prime_test(10) == 1
//                  ? "Is prime acc. to Solovay Strassen"
//                  : "Is not prime acc. Solovay Strassen")
//              << std::endl;
//
//    number = ext_base_t(2);
//    std::cout << number << std::endl;
//    std::cout << (number.fermat_prime_test(10) == 1
//                  ? "Is prime acc. to Fermat"
//                  : "Is not prime acc. to Fermat")
//              << std::endl;
//    std::cout << (number.solovay_strassen_prime_test(10) == 1
//                  ? "Is prime acc. to Solovay Strassen"
//                  : "Is not prime acc. Solovay Strassen")
//              << std::endl;
}