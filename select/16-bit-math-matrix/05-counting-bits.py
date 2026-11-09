# LC 338 Counting Bits (Easy). Pattern: DP on i >> 1.
# For every i in [0, n], return the number of 1 bits in i.


# i has the bits of i // 2 plus its own lowest bit. T: O(n), S: O(n) for the output.
def count_bits(n):
    # bits[i] = number of set bits in i
    bits = [0] * (n + 1)
    for i in range(1, n + 1):
        bits[i] = bits[i >> 1] + (i & 1)
    return bits


def test(label, n, want):
    got = count_bits(n)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("zero", 0, [0])
    test("five", 5, [0, 1, 1, 2, 1, 2])
    test("eight", 8, [0, 1, 1, 2, 1, 2, 2, 3, 1])


main()

# follow-up: other recurrence -> bits[i] = bits[i & (i - 1)] + 1 (drops the lowest 1)
# follow-up: one number (LC 191) -> loop n &= n - 1 and count; runs once per set bit
#   (mask n with & 0xFFFFFFFF first if it can be negative)
