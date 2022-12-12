import sys

def factor(num: int) -> int | None:
    """
    Находит нетривиальный делитель числа `num` методом Олвея 
    в диапазоне (2 * num**(1/3); num**(1/2)]
    """
    
    d = 2 * int(num ** (1/3)) + 1  

    r1 = num % d
    r2 = num % (d - 2)

    q1 = num // d
    q2 = num // (d - 2)

    q = 4 * (q2 - q1)
    s = int(num ** (1/2))

    while r1 != 0:
        d += 2
        if d > s: 
            return None
        
        r = 2 * r1 - r2 + q
        r2, r1 = r1, r

        if r1 < 0:
            r1 += d
            q += 4

        while r1 >= d:
            r1 -= d
            q -= 4
    # end while r1 != 0

    return d


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} number-to-factor")
        return

    to_factor = int(sys.argv[1])
    one_factor = factor(to_factor)

    l = int(2*to_factor**(1/3))
    h = int(to_factor**(1/2))
    print(f"Bounds for searching: ({l}; {h}]")
    print(f"Found factor: {one_factor}")

    # TEST:
    from sympy.ntheory import factorint
    
    print(f"Real factors: {factorint(to_factor)}")
    if one_factor is not None:
        print(f"Found factors: {factorint(one_factor)}")


if __name__ == "__main__":
    main()