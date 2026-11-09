# LC 162 Find Peak Element (Medium). Pattern: binary search on the slope.
# Return the index of any peak (strictly greater than its neighbors), where
# nums[-1] = nums[n] = -inf and adjacent values differ, in O(log n).


# Always move toward the higher neighbor; a peak stays inside [lo, hi].
# T: O(log n), S: O(1).
def find_peak_element(nums):
    lo = 0
    hi = len(nums) - 1
    while lo < hi:
        mid = (lo + hi) // 2
        # mid < hi, so mid + 1 is in range
        if nums[mid] < nums[mid + 1]:
            # rising: climbing right must hit a peak (nums[n] = -inf)
            lo = mid + 1
        else:
            # falling: mid or something left of it is a peak
            hi = mid
    return lo


def test(label, nums, want):
    got = find_peak_element(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("middle", [1, 3, 2], 1)
    test("left-edge", [3, 2, 1], 0)
    test("right-edge", [1, 2], 1)
    test("single", [5], 0)


main()

# follow-up: 2D peak (LC 1901) -> binary search on columns; take the max of column mid,
#   move toward its larger horizontal neighbor, O(m log n)
