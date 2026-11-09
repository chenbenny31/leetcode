# LC 34 Find First and Last Position of Element in Sorted Array (Medium). Pattern:
# lower-bound template.
# nums sorted ascending; return [first, last] index of target, or [-1, -1], in O(log n).


# Lower-bound template: first index i with nums[i] >= target (n if none).
# The answer always lies in [lo, hi]; hi starts at n so "none" is a valid answer.
# T: O(log n), S: O(1).
def lower_bound(nums, target):
    lo = 0
    hi = len(nums)
    while lo < hi:
        # Python ints don't overflow, so (lo + hi) // 2 is safe
        mid = (lo + hi) // 2
        if nums[mid] < target:
            # mid and everything left of it are too small
            lo = mid + 1
        else:
            # mid is a candidate; the answer is mid or left of it
            hi = mid
    return lo


# first = lower_bound(target), last = lower_bound(target + 1) - 1. T: O(log n), S: O(1).
def search_range(nums, target):
    n = len(nums)
    first = lower_bound(nums, target)
    if first == n or nums[first] != target:
        return [-1, -1]
    # last target sits just before the first value > target
    last = lower_bound(nums, target + 1) - 1
    return [first, last]


def test(label, nums, target, want):
    got = search_range(nums, target)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("dups", [1, 2, 2, 2, 3], 2, [1, 3])
    test("all-same", [2, 2], 2, [0, 1])
    test("missing-gap", [1, 3], 2, [-1, -1])
    test("empty", [], 1, [-1, -1])


main()

# follow-up: upper bound (first > target) -> same loop with nums[mid] <= target;
#   count of target = upper_bound - lower_bound; insert position (LC 35) = lower_bound;
#   stdlib: bisect.bisect_left / bisect.bisect_right
# follow-up: "first index where pred is true" for any monotonic pred -> same template,
#   replace nums[mid] < target with not pred(mid)
