# LC 209 Minimum Size Subarray Sum (Medium). Pattern: variable window, shrink while
# valid.
# nums all positive; return the minimal length of a contiguous subarray with sum
# >= target, or 0 if none.


# All values positive, so growing right raises the sum and shrinking left lowers it.
# T: O(n), S: O(1).
def min_sub_array_len(target, nums):
    n = len(nums)
    # n + 1 = no valid window yet
    best = n + 1
    # sum of nums[left..right]
    window_sum = 0
    left = 0
    for right in range(n):
        window_sum += nums[right]
        while window_sum >= target:
            best = min(best, right - left + 1)
            window_sum -= nums[left]
            left += 1
    if best == n + 1:
        return 0
    return best


def test(label, target, nums, want):
    got = min_sub_array_len(target, nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("pair", 5, [1, 4, 1], 2)
    test("one-element", 4, [1, 1, 4], 1)
    test("whole-array", 6, [1, 2, 3], 3)
    test("impossible", 10, [1, 2, 3], 0)


main()

# follow-up: O(n log n) (LC asks) -> prefix[i] = sum of nums[0..i-1] is increasing;
#   for each i, j = bisect_left(prefix, prefix[i] + target); if j <= n: best = j - i
# follow-up: negatives allowed (LC 862) -> monotonic deque of prefix indices, O(n)
