# LC 215 Kth Largest Element in an Array (Medium). Pattern: k-min-heap / quickselect.
# Return the k-th largest element of nums (k-th in sorted order, duplicates count).

import heapq
import random


# Keep the k largest seen so far; their min is the answer. T: O(n log k), S: O(k).
def find_kth_largest(nums, k):
    # k largest so far, smallest on top
    heap = []
    for num in nums:
        heapq.heappush(heap, num)
        if len(heap) > k:
            heapq.heappop(heap)
    return heap[0]


# Randomized quickselect with 3-way partition (duplicates never stall it).
# T: O(n) average, O(n^2) worst, S: O(1) extra (nums is a copy).
def quick_select(nums, k):
    # copy: the caller's list must not be reordered
    nums = list(nums)
    n = len(nums)
    # index of the answer if nums were sorted ascending
    target = n - k
    # the answer stays inside [lo, hi]
    lo = 0
    hi = n - 1
    while lo < hi:
        # randint is inclusive on both ends
        pivot_index = random.randint(lo, hi)
        pivot = nums[pivot_index]
        # [lo, lt) < pivot, [lt, i) == pivot, [i, gt] unseen, (gt, hi] > pivot
        lt = lo
        i = lo
        gt = hi
        while i <= gt:
            if nums[i] < pivot:
                nums[lt], nums[i] = nums[i], nums[lt]
                lt += 1
                i += 1
            elif nums[i] > pivot:
                # i stays: the value swapped in from gt is not classified yet
                nums[i], nums[gt] = nums[gt], nums[i]
                gt -= 1
            else:
                i += 1
        if target < lt:
            hi = lt - 1
        elif target > gt:
            lo = gt + 1
        else:
            return nums[target]
    # lo == hi == target: a one-element range is already in place
    return nums[target]


def test(label, nums, k, want):
    ok = find_kth_largest(nums, k) == want and quick_select(nums, k) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("distinct", [7, 1, 5, 3], 2, 5)
    test("dups", [5, 5, 1, 3], 3, 3)
    test("all-equal", [7, 7, 7, 7], 2, 7)
    test("negatives", [-1, -5, -3], 3, -5)


main()

# follow-up: worst-case O(n) -> median-of-medians pivot (groups of 5); Python has no
#   introselect, heapq.nlargest(k, nums)[-1] is the O(n log k) one-liner
# follow-up: top k elements -> after quickselect nums[n-k:] holds them, O(n)
