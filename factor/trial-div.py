from sympy.ntheory import isprime
from collections import defaultdict
from math import sqrt
from pprint import pprint
import sys

def factor(num: int) -> dict[int, int]:
    factors = defaultdict(int)
    
    if isprime(num) or -1 <= num <= 1:
        factors[num] = 1
        return factors

    if num < 0:
        factors[-1] = 1
        num = abs(num)    
    
    while num % 2 == 0:
        factors[2] += 1
        num //= 2

    divs = [3, 5, 7]
    bound = sqrt(num)
    while divs[0] <= bound:
        if num % divs[0] != 0:
            new_div = divs[0] + 6
            divs[0] = divs[1]
            divs[1] = divs[2]
            divs[2] = new_div
            continue

        factors[divs[0]] += 1
        num //= divs[0]
    
    if num != 1:
        factors[num] += 1

    return factors


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} number-to-factor")
        return

    to_factor = int(sys.argv[1])
    factors = factor(to_factor)
    pprint(dict(factors))

    # TEST:
    from sympy.ntheory import factorint
    expected = factorint(to_factor)

    print("Expected:")
    pprint(expected)

    if expected != factors:
        print("Test failed")
    else:
        print("Test passed")


if __name__ == "__main__":
    main()