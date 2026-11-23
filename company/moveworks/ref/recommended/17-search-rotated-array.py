# 17 · Search in Rotated Sorted Array (ServiceNow phone 2024, Medium) — LC 33
# ascending distinct nums rotated at an unknown pivot; return the idx of target or -1;
# O(log n)
#
# clarify:
# distinct values? -> yes; dups = follow-up (LC 81)
# rotated 0 times -> ok, plain sorted array
# empty array -> -1


# core idea: one half of [lo, hi] around mid is always sorted; go into it only if
# target is inside its value range, else into the other half
# T: O(log n), S: O(1)
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
    ok = search(nums, target) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("right-half", [5, 6, 7, 1, 2, 3], 2, 4)
    test("left-half", [5, 6, 7, 1, 2, 3], 6, 1)
    test("missing", [5, 6, 7, 1, 2, 3], 4, -1)
    test("two-elements", [3, 1], 1, 1)


main()

# follow-up:
# 1. dups allowed (LC 81) -> if nums[lo] == nums[mid] == nums[hi]: lo += 1, hi -= 1
# 2. two-pass version -> find the idx of the min (LC 153), then bisect the right part
