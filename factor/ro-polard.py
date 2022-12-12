from sympy.ntheory import isprime
from pprint import pprint
from math import gcd
import sys


def factor(n: int) -> int:
    f = lambda a: (a ** 2 + 1) % n
    d = 1
    a = b = 2
    while d == 1:
        a, b = f(a), f(f(b))  # Floyd's method
        if a == b:
            # f = lambda a: (a ** 2 + random.randint(0, n-1)) % n
            return None
        d = gcd(abs(a-b), n)
    return d


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} number-to-factor")
        return

    to_factor = int(sys.argv[1])

    if isprime(to_factor):
        print("Given number is prime")
        return

    d = factor(to_factor)
    if d is None:
        print("Couldn't find divisors!")
    else:
        print(d)

    # TEST:
    from sympy.ntheory import factorint
    expected = factorint(to_factor)
    print("Real: ", end='')
    pprint(expected)


if __name__ == "__main__":
    main()