# LC 152 Maximum Product Subarray (Medium). Pattern: Kadane with max and min.
# Return the largest product of a non-empty contiguous subarray of nums (may contain
# negatives and zeros).


# Track the max and min product ending here; a negative swaps them. T: O(n), S: O(1).
def max_product(nums):
    # largest / smallest product of a subarray ending at the current element
    max_here = nums[0]
    min_here = nums[0]
    best = nums[0]
    n = len(nums)
    for i in range(1, n):
        num = nums[i]
        # times a negative, the smallest product becomes the largest
        if num < 0:
            max_here, min_here = min_here, max_here
        # num alone restarts the subarray (e.g. after a zero)
        max_here = max(num, max_here * num)
        min_here = min(num, min_here * num)
        best = max(best, max_here)
    return best


def test(label, nums, want):
    got = max_product(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two-negatives", [-2, 3, -4], 24)
    test("zero-split", [2, 0, 3, 4], 12)
    test("zero-beats-neg", [-1, 0, -2], 0)
    test("single-negative", [-3], -3)


main()

# follow-up: max sum subarray (LC 53) -> same scan with only max_here:
#   max_here = max(num, max_here + num)
# follow-up: return the subarray -> remember where max_here last restarted (== num) and
#   record [start, i] whenever best improves; same for min_here, swapped together
