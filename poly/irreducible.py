import sys
from sympy import Poly
from sympy.parsing.sympy_parser import parse_expr
from sympy import GF
from sympy.abc import x
from sympy import gcdex


def gcd(f: Poly, g: Poly) -> Poly:
    while g.degree() >= 0:
        f, g = g, f % g
    return f.monic()


def is_irreducible(f: Poly, p: int) -> bool:
    n = f.degree()
    u = Poly(x, domain=f.domain)
    m = Poly(x, domain=f.domain)
    for i in range(0, n//2):
        u = (u ** p) % f
        d = gcd(f, u - m)
        if d != 1:
            return False
    return True


def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <polynomial-as-string> <p>")
        return

    p = int(sys.argv[2])
    poly = Poly(parse_expr(sys.argv[1]), domain=GF(p))
    print(f"Is {poly} irreducible over GF({p})? {is_irreducible(poly, p)}")

    # TEST:
    print("\nTesting with sympy...")
    print(f"Is {poly} irreducible over GF({p})? {poly.is_irreducible}")


if __name__ == "__main__":
    main()