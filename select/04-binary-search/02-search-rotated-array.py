# LC 33 Search in Rotated Sorted Array (Medium). Pattern: binary search, one sorted half.
# nums is ascending with distinct values, rotated at an unknown pivot; return the index
# of target, or -1, in O(log n).


# One half of [lo, hi] around mid is always sorted; keep it only if target lies inside
# its range. T: O(log n), S: O(1).
def search(nums, target):
    lo = 0
    hi = len(nums) - 1
    while lo <= hi:
        mid = (lo + hi) // 2
        if nums[mid] == target:
            return mid
        # <=: when lo == mid the left half is the single sorted value nums[lo]
        if nums[lo] <= nums[mid]:
            # left half [lo, mid] is sorted
            if nums[lo] <= target < nums[mid]:
                hi = mid - 1
            else:
                lo = mid + 1
        else:
            # right half [mid, hi] is sorted
            if nums[mid] < target <= nums[hi]:
                lo = mid + 1
            else:
                hi = mid - 1
    return -1


def test(label, nums, target, want):
    got = search(nums, target)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("right-half", [5, 6, 7, 1, 2, 3], 2, 4)
    test("left-half", [5, 6, 7, 1, 2, 3], 6, 1)
    test("missing", [5, 6, 7, 1, 2, 3], 4, -1)
    test("two-elements", [3, 1], 1, 1)


main()

# follow-up: duplicates (LC 81) -> if nums[lo] == nums[mid] == nums[hi], neither half
#   is known sorted: lo += 1, hi -= 1; O(n) worst case
# follow-up: two-pass -> find the min index (LC 153), then plain binary search on the
#   sorted part that can hold target
