from math import gcd
from sympy import totient as phi
import sys

DEBUG = None
def dprint(m):
    if DEBUG:
        print(m)


def _F(a, g, p):
    n = phi(p)

    t = p // 3
    dprint(f"{t=}, {2*t=}, {p=}")
    def F(x, y, b):
        match x:
            case x if 1 <= x < t:
                x = (a * x) % p
                y = y
                b = (b + 1) % n
                return (x, y, b)
            case x if t <= x < 2*t:
                x = (x ** 2) % p
                y = (2 * y) % n
                b = (2 * b) % n
                return (x, y, b)
            case x if 2*t <= x < p:
                x = (g * x) % p
                y = (y + 1) % n
                b = b
                return (x, y, b)
            case _:
                # UNREACHABLE
                return None 
    return F


def solve(a, b, n):
    """
        Solve equation ax=b (mod n)
    """
    dprint(f"{a}x = {b} (mod {n})")
    d = gcd(a, n)
    if b % d != 0:
        return None

    a0 = a // d
    b0 = b // d
    n0 = n // d

    x0 = (pow(a0, -1, n0) * b0) % n0
    
    solutions = [x0 + i * n0 for i in range(d)]
    return solutions


def dlog(a, g, p):
    F = _F(a, g, p)

    x1 = x2 = 1
    y1 = y2 = 0
    b1 = b2 = 0

    dprint(f"{x1=},\t{y1=},\t{b1=},\t{x2=},\t{y2=},\t{b2=}")
    x1, y1, b1 = F(x1, y1, b1)
    x2, y2, b2 = F(*F(x2, y2, b2))
    dprint(f"{x1=},\t{y1=},\t{b1=},\t{x2=},\t{y2=},\t{b2=}")
    while x1 != x2:
        x1, y1, b1 = F(x1, y1, b1)
        x2, y2, b2 = F(*F(x2, y2, b2))
        dprint(f"{x1=},\t{y1=},\t{b1=},\t{x2=},\t{y2=},\t{b2=}")

    n = phi(p)
    r = (b1 - b2) % n
    if r == 0:
        return None  # This method is not useful here.

    solutions = solve(r, (y2 - y1) % n, n)
    if solutions is None:
        return None
    
    for z in solutions:
        dprint(f"{g} ** {z} = {(g**z)%p} ?= {a}")
        if (g ** z) % p == a:
            return z

    return None

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <element> <gen> <order>")
        return

    a = int(sys.argv[1]) 
    g = int(sys.argv[2])
    p = int(sys.argv[3])
    
    a %= p
    x = dlog(a, g, p)
    if x is None:
        print("☠️ Not found")
        return
    print(f"Found exponent: {x=}")

    # TEST:
    print()
    if (g ** x) % p == a:
        print("✅ TEST PASSED")
    else:
        print("❌ TEST FAILED")


if __name__ == "__main__":
    main()