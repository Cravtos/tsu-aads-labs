from irreducible import is_irreducible
from sympy import Poly
from sympy.abc import x
from sympy import GF
from sympy.ntheory import factorint
from sympy.parsing.sympy_parser import parse_expr
import sys


def is_primitive(f: Poly, p: int) -> bool:
    n = f.degree()
    m = p ** n - 1
    a = Poly(x, domain=f.domain)

    factors = factorint(m)
    for factor in factors:
        if factor == 1:
            continue
        r = (a ** (m // factor)) % f
        if r == 1:
            return False
    return True



def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <polynomial-as-string> <p>")
        return

    p = int(sys.argv[2])
    poly = Poly(parse_expr(sys.argv[1]), domain=GF(p))

    if not is_irreducible(poly, p):
        print(f"{poly} is not irreducible over GF({p})")
        return

    print(f"Is {poly} primitive over GF({p})? {is_primitive(poly, p)}")


if __name__ == "__main__":
    main()