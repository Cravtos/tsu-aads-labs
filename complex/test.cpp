#include "complex.hpp"

void test_streams()
{
    std::cout << std::endl << "Function 'test_streams()' started: " << std::endl;
    Complex comp;
    std::cout << "You need to enter two values: real and imaginary part of number: ";
    std::cin >> comp;
    std::cout << comp << std::endl;
}

void test_operations()
{
    std::cout << std::endl << "Function 'test_operations()' started: " << std::endl;

    Complex comp1(3, -5);
    Complex comp2(-3, 2);

    std::cout << "comp1 = "<< comp1 << std::endl;
    std::cout << "comp2 = "<< comp2 << std::endl;

    std::cout << "-comp1 = " << -comp1 << std::endl;
    std::cout << "comp1.conj() = "<< comp1.conj() << std::endl;
    std::cout << "comp1.abs() = "<< comp1.abs() << std::endl;
    std::cout << "comp1 / 3 = "<< comp1 / 3 << std::endl;
    std::cout << "comp1 * comp2 = "<< comp1 * comp2 << std::endl;
    std::cout << "comp1 * 3 = "<< comp1 * 3 << std::endl;
    std::cout << "comp1 + comp2 = "<< comp1 + comp2 << std::endl;
    std::cout << "comp1 - comp2 = "<< comp1 - comp2 << std::endl;
    std::cout << "comp1 / comp2 = "<< comp1 / comp2 << std::endl;

    comp2 -= comp1;
    std::cout << "comp2 -= comp1 is " << comp2 << std::endl;

    comp2 += comp1;
    std::cout << "comp2 += comp1 is " << comp2 << std::endl;

    comp2 /= comp1;
    std::cout << "comp2 /= comp1 is " << comp2 << std::endl;

    comp2 *= comp1;
    std::cout << "comp2 *= comp1 is " << comp2 << std::endl;

    comp2 /= 3;
    std::cout << "comp2 /= 3 is " << comp2 << std::endl;

    comp2 *= 2;
    std::cout << "comp2 *= 2 is " << comp2 << std::endl;

    comp2 += 2;
    std::cout << "comp2 += 2 is " << comp2 << std::endl;

    comp2 -= 12.0;
    std::cout << "comp2 -= 12.0 is " << comp2 << std::endl;
}

void test_constructors()
{
    std::cout << std::endl << "Function 'test_constructors()' started: " << std::endl;

    Complex comp1(1, 2);
    Complex comp2(1);
    Complex comp3;

    std::cout << "comp1: " << comp1 << std::endl;
    std::cout << "comp2: " << comp2 << std::endl;
    std::cout << "comp3: " << comp3 << std::endl;
}

int main(int argc, char* argv[])
{
    test_streams();
    test_operations();
    test_constructors();

    return 0;
}
