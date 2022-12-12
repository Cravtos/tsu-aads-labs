from math import sqrt
import sys

DEBUG = False
def dprint(m):
    if DEBUG:
        print(m)

def dlog(a, g, n):
    h = int(sqrt(n)) + 1
    b = pow(g, h, n)

    dprint(f"{h=}")
    dprint(f"{b=}")

    prev_gs = 1
    giant_steps = {}
    for u in range(1, h+1):
        gs = (prev_gs * b) % n
        giant_steps[gs] = u
        prev_gs = gs

    dprint(f"{giant_steps=}")

    prev_bs = a
    for v in range(1, n+1):
        bs = (prev_bs * g) % n
        dprint(f"{v=}, {bs=}")
        prev_bs = bs
        u = giant_steps.get(bs)
        if u is not None:
            return (h * u - v) % n
    
    # UNREACHABLE
    return None 

def main():
    if len(sys.argv) != 4:
        print(f"Usage: {sys.argv[0]} <element> <gen> <order>")
        return

    a = int(sys.argv[1]) 
    g = int(sys.argv[2])
    n = int(sys.argv[3])

    a %= n

    x = dlog(a, g, n)
    if x is None:
        print("☠️ Something broke")
        return
    print(f"Found exponent: {x=}")

    # TEST:
    print()
    if (g ** x) % n == a:
        print("✅ TEST PASSED")
    else:
        print("❌ TEST FAILED")


if __name__ == "__main__":
    main()