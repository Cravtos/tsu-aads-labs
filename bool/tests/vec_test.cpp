#include "../include/bool_vec.h"

void constructors_test() {
    std::cout << std::endl << "test constructor from string" << std::endl;
    const std::string str = "11101110101";
    auto from_str = BoolVec(str);
    std::cout << from_str << std::endl;
    std::cout << "should be equal to: \n" << str << std::endl;

    std::cout << std::endl << "test constructor from size" << std::endl;
    const size_t size = 10;
    auto from_size = BoolVec(size);
    if (size == from_size.size()) {
        std::cout << from_size << std::endl;
        std::cout << "OK" << std::endl;
    }

    std::cout << std::endl << "test copy constructor" << std::endl;
    auto copy = BoolVec(from_str);
    if (copy == from_str) {
        std::cout << copy << std::endl;
        std::cout << "OK" << std::endl;
    }

    std::cout << std::endl << "test operator=" << std::endl;
    copy = from_str;
    if (copy == from_str) {
        std::cout << copy << std::endl;
        std::cout << "OK" << std::endl;
    }
}

void shifts_test() {
    std::cout << std::endl << "test shifts" << std::endl;
    const std::string str = "111011101010000000000000000000000111111010101001111010101";
    auto from_str = BoolVec(str);

    const size_t tests = 100;
    for (size_t i = 0; i < tests; i += 5) {
        auto copy = from_str;
        from_str <<= i;
        from_str >>= i;
        if (!(copy == from_str)) {
            std::cout << "test shift failed for shift " << i;
            std::cout << "\ncpy\n" << copy << "\nsrc\n" << from_str<< std::endl;
            return;
        }
        std::cout << "ok for shift " << i << std::endl;
    }
    std::cout << "test shifts finished successfully" << std::endl;
}

void read_test() {
    std::cout << std::endl << "test read" << std::endl;

    try {
        auto from_stdin = BoolVec();
        std::cout << "enter vector: " << std::endl;
        std::cin >> from_stdin;
        std::cout << from_stdin;
    }
    catch (const std::invalid_argument&) {
        std::cout << "should be binary representing string" << std::endl;
    }
}

void formulas_test() {
    // morgan
    auto first = BoolVec("0101010011110010101010010101001010101010010");
    auto second = BoolVec("1110101011110101001011111110100000000000101");
    auto lhs = ~(first | second);
    auto rhs = (~first) & (~second);
    if (!(lhs == rhs)) {
        std::cout << "morgan test failed" << std::endl;
        return;
    }
    std::cout << "morgan OK" << std::endl;

    // xor itself
    auto vecxor = BoolVec("010101001111001010101001010100101010101001010101010100101010100101010101001");
    auto newvec = vecxor ^ vecxor;
    if (newvec.weight() != 0) {
        std::cout << "xor itself failed" << std::endl;
        return;
    }
    std::cout << "xor OK" << std::endl;


    // double inverse
    auto inv = BoolVec("0101");
    auto copy = ~(~inv);
    if (!(inv == copy)) {
        std::cout << "double inverse failed" << std::endl;
        return;
    }
    std::cout << "double inv OK" << std::endl;
}

int main() {
    constructors_test();
    shifts_test();
    read_test();
    formulas_test();

    return 0;
}