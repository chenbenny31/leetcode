# 13 · Find K Closest Elements (phone screen, Medium) — LC 658, binary search on start
# sorted arr; return the k values closest to x, sorted; a tie goes to the smaller value
#
# clarify:
# arr sorted ascending, may have duplicates -> yes
# 1 <= k <= n -> yes
# x may be outside the range of arr -> yes
# output order -> ascending


# core idea: the answer is a window arr[lo:lo + k]; binary search its start: if
# arr[mid + k] is strictly closer than arr[mid], the window must start after mid
# T: O(log(n - k) + k), S: O(1) besides the output
def find_closest_elements(arr, k, x):
    lo = 0
    # last possible start
    hi = len(arr) - k
    while lo < hi:
        mid = (lo + hi) // 2
        # compare signed distances, no abs: a tie keeps the left (smaller) value
        if x - arr[mid] > arr[mid + k] - x:
            lo = mid + 1
        else:
            hi = mid
    return arr[lo:lo + k]


def test(label, arr, k, x, want):
    ok = find_closest_elements(arr, k, x) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("tie-smaller", [1, 3, 5, 7, 9], 1, 6, [5])
    test("left-of-all", [1, 3, 5, 7, 9], 2, 0, [1, 3])
    test("right-of-all", [1, 3, 5, 7, 9], 2, 20, [7, 9])
    test("duplicates", [1, 1, 2, 2, 2, 2, 2, 3, 3], 3, 3, [2, 3, 3])


main()

# follow-up:
# 1. why not abs -> duplicates make an abs tie keep the left window wrongly
# 2. O(n) way -> two pointers from both ends, drop the farther one until k left
# 3. unsorted input -> heapq.nsmallest(k, arr, key=lambda v: (abs(v - x), v)), sort
# 4. bisect version -> bisect_left for the insertion point, expand left / right k times
