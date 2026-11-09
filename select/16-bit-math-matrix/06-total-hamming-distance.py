# LC 477 Total Hamming Distance (Medium). Pattern: count ones per bit position.
# Return the sum of Hamming distances over all pairs of nums (0 <= nums[i] < 2^31).


# At each bit, every (one, zero) pair differs: add ones * zeros. T: O(31n), S: O(1).
def total_hamming_distance(nums):
    n = len(nums)
    total = 0
    for bit in range(31):
        # numbers with this bit set
        ones = 0
        for num in nums:
            ones += (num >> bit) & 1
        total += ones * (n - ones)
    return total


def test(label, nums, want):
    got = total_hamming_distance(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("mixed", [1, 2, 3], 4)
    test("high-bit", [0, 1 << 30], 1)
    test("all-equal", [7, 7, 7], 0)
    test("single", [5], 0)


main()

# follow-up: brute force -> popcount(a ^ b) for every pair, O(32 * n^2); flipping the
#   loops to per-bit counting removes the pairs
