import sys
from pprint import pprint

def factor(num: int):
    """
    Находит нетривиальные делити числа `num` методом Ферма
    """

    x = int(num ** (1/2))
    if x ** 2 == num:
        return x

    while True:
        x += 1

        if x == (num + 1) // 2:
            return num

        z = x ** 2 - num
        y = int(z ** (1/2))

        if y ** 2 == z:
            return (x + y, x - y)


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} number-to-factor")
        return

    to_factor = int(sys.argv[1])

    a, b = factor(to_factor)
    print(f"Found factors: {(a, b)}")

    # # TEST:
    # from sympy.ntheory import factorint
    # expected = factorint(to_factor)

    # print("Real factors:")
    # pprint(expected)
    # print(f"Found factors: {(factorint(a), factorint(b))}")


if __name__ == "__main__":
    main()