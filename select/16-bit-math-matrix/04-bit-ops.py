# Apple Bit Operations (Easy). Pattern: bit masks, x & (x - 1).
# (1) Given unsigned x, bit index i (from the LSB) and op "flip" or "clear", return x
# with op applied to bit i. (2) Count the set bits of a 32-bit unsigned value without
# strings or builtins.


# flip = XOR with the mask, clear = AND with the inverted mask. T: O(1), S: O(1).
def apply_op(x, i, op):
    # only bit i set
    mask = 1 << i
    if op == "flip":
        return x ^ mask
    elif op == "clear":
        # ~mask is negative in Python (infinite ones); & 0xFFFFFFFF keeps it a uint32
        return x & ~mask & 0xFFFFFFFF
    return x


# Test each of the 32 bits. T: O(32), S: O(1).
def count_bits(x):
    count = 0
    for bit in range(32):
        count += (x >> bit) & 1
    return count


# x & (x - 1) drops the lowest set bit: one loop per set bit. T: O(set bits), S: O(1).
def count_bits_fast(x):
    # view x as uint32: a negative Python int has infinite ones and would never hit 0
    x &= 0xFFFFFFFF
    count = 0
    while x != 0:
        x &= x - 1
        count += 1
    return count


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("flip", apply_op(0b1010, 1, "flip"), 0b1000)
    test("clear", apply_op(0b1010, 3, "clear"), 0b0010)
    test("clear-zero-bit", apply_op(0b1010, 0, "clear"), 0b1010)
    test("count", count_bits(0b10110000), 3)
    test("count-fast", count_bits_fast(0b10110000), 3)
    # C++ ~0u; in Python ~0 is -1, so the 32-bit all-ones value is written out
    test("all-ones", count_bits_fast(0xFFFFFFFF), 32)


main()

# follow-up: more ops -> set: x | mask, test: (x >> i) & 1
# follow-up: why C++ needs 1u, not 1 -> 1 << 31 overflows a signed int (undefined
#   behavior); Python ints never overflow, so the risk flips: results can grow past 32
#   bits or go negative (~, <<), mask with & 0xFFFFFFFF where that matters
