from sympy.ntheory import isprime, nextprime
from math import sqrt
from pprint import pprint
from collections import defaultdict
from z3 import BitVec, Solver, sat, Sum, Extract
from math import gcd
import sys


DEBUG = False


def dprint(msg):
    if DEBUG == True:
        print(msg)


def gen_factor_base(num, factor_base_len):
    is_residue = lambda a, p: (a ** ((p - 1) // 2)) % p == 1
    factor_base = [-1, 2]

    prev_prime = factor_base[-1]
    while len(factor_base) < factor_base_len:
        prime = nextprime(prev_prime)
        if is_residue(num, prime):
            factor_base.append(prime)
        prev_prime = prime

    return factor_base


def decomp_exist(factor_base, num):
    factors = defaultdict(int)
    for factor in factor_base:
        if factor == -1:
            if num < 0:
                num = -num
                factors[-1] = 1
            continue

        while num % factor == 0:
            factors[factor] += 1
            num /= factor

    if num == 1:
        return factors

    return None


def x_gen():
    x = 0
    yield x
    while True:
        x += 1
        yield x
        x *= -1
        yield x
        x *= -1


def gen_pairs(factor_base, amount, m, num):
    pairs = []
    b_decomps = []
    xg = x_gen()
    while len(pairs) < amount:
        x = next(xg)
        a = x + m
        b = (x + m) ** 2 - num
        b_decomp = decomp_exist(factor_base, b)
        dprint(f"x = {x}; a = {a}, b = {b}, b_decomp = {b_decomp}")
        if b_decomp is None:
            continue
        pairs.append([a, b])
        b_decomps.append(b_decomp)
    
    return pairs, b_decomps


def solve(pow_mat, n):
    s = Solver()
    c = BitVec('c', n)
    s.add(c != 0)
    for pow_vec in pow_mat:
        t = int(''.join(str(bit) for bit in pow_vec), 2)
        t = c & t
        bit_sum = Sum([ Extract(i,i,t) for i in range(n) ])
        s.add(bit_sum % 2 == 0)

    while s.check() == sat:
        m = s.model()
        sol = m[c].as_binary_string()
        sol = '0'*(n-len(sol)) + sol
        yield sol
        s.add(c != m[c])


def check_sol(sol, pairs, b_decomps, num):
    dprint(f"Current solution: {sol}")
    idxs = [i for i, b in enumerate(sol) if b == '1']
    
    y_decomp = defaultdict(int)

    b = 1
    for idx in idxs:
        b *= pairs[idx][1]
        for factor, power in b_decomps[idx].items():
            y_decomp[factor] += power
    x = 1
    for idx in idxs:
        x *= pairs[idx][0] % num
    
    # y = int(sqrt(b))
    y = 1
    for factor, power in y_decomp.items():
        y *= factor ** (power // 2)

    dprint(f"b = {b}; x = {x}; y = {y}")

    if (x - y) % num != 0 and (x + y) % num != 0:
        return gcd(x+y, num)

    return None


def factor(num: int, factor_base_len: int) -> int:
    if factor_base_len < 3:
        raise Exception("Invalid value for factor_base_length: should not be less than 2")

    factor_base = gen_factor_base(num, factor_base_len)
    dprint(f"Factor base = {factor_base}")

    
    pairs_amount = factor_base_len + 1
    m = int(sqrt(num))

    dprint(f"Pairs amount = {pairs_amount}")
    dprint(f"m = {m}")

    pairs, b_decomps = gen_pairs(factor_base, pairs_amount, m, num)
    dprint(f"Resulting pairs, b_decomps = {list(zip(pairs, b_decomps))}")
    
    pow_mat = [[decomp[factor] % 2 for decomp in b_decomps] for factor in factor_base]  # V^T
    dprint(f"V = {[[decomp[factor] % 2 for factor in factor_base] for decomp in b_decomps]}")

    sg = solve(pow_mat, pairs_amount)
    for sol in sg:
        d = check_sol(sol, pairs, b_decomps, num)
        if d is not None:
            return d

    return None


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} number-to-factor")
        return

    num = int(sys.argv[1])
    if isprime(num):
        print("Number is prime")
        return

    if num % 2 == 0:
        print("Number is even")
        return

    factor_base_len = 30
    dprint(f"Factoring with factor base length = {factor_base_len}")
    d = factor(num, factor_base_len)
    while d is None:
        factor_base_len += 1
        dprint(f"Factoring with factor base length = {factor_base_len}")
        d = factor(num, factor_base_len)

    print(f"Result: {d}")

    # TEST:
    from sympy.ntheory import factorint
    print(factorint(d))
    
    expected = factorint(num)

    print("\nExpected:")
    pprint(expected)


if __name__ == "__main__":
    main()