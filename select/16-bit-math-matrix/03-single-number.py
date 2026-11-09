# LC 136 Single Number (Easy). Pattern: XOR cancels pairs.
# Every element appears twice except one; return that one in O(n) time and O(1)
# extra space.


# XOR everything: a ^ a = 0 and 0 ^ x = x, order doesn't matter. T: O(n), S: O(1).
def single_number(nums):
    single = 0
    for num in nums:
        single ^= num
    return single


def test(label, nums, want):
    got = single_number(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("middle", [4, 9, 4], 9)
    test("alone", [7], 7)
    test("negative", [-3, 5, 5, 8, 8], -3)


main()

# follow-up: others appear three times (LC 137) -> per bit count set bits mod 3; the
#   leftover bits form the answer, O(32n); in Python bit 31 set means negative:
#   if result >= 2**31: result -= 2**32 (no int32 wraparound to do it for you)
# follow-up: two singles (LC 260) -> x = xor of all; split nums by the lowest set bit
#   of x (low = x & -x, no unsigned cast needed in Python) and xor each group
