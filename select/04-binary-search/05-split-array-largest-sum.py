# LC 410 Split Array Largest Sum (Hard). Pattern: binary search on the answer + greedy.
# Split nums (non-negative) into k non-empty contiguous subarrays so that the largest
# subarray sum is minimized; return that sum.


# Greedy: fewest subarrays so that no subarray sum exceeds cap (cap >= max element).
# T: O(n), S: O(1).
def pieces_needed(nums, cap):
    pieces = 1
    # sum of the current piece
    piece_sum = 0
    for num in nums:
        if piece_sum + num > cap:
            pieces += 1
            piece_sum = 0
        piece_sum += num
    return pieces


# Lower-bound template on cap in [max element, total]: pieces <= k is monotonic in cap,
# and fewer than k pieces can always be split further. T: O(n log(total)), S: O(1).
def split_array(nums, k):
    lo = max(nums)
    hi = sum(nums)
    while lo < hi:
        mid = (lo + hi) // 2
        if pieces_needed(nums, mid) <= k:
            hi = mid
        else:
            lo = mid + 1
    return lo


def test(label, nums, k, want):
    got = split_array(nums, k)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("two-parts", [1, 2, 3, 4], 2, 6)
    test("one-part", [1, 2, 3, 4], 1, 10)
    test("each-alone", [1, 2, 3, 4], 4, 4)
    test("big-first", [9, 1, 1, 1], 2, 9)


main()

# follow-up: DP alternative -> dp[i][j] = best for first i values in j parts,
#   min over split p of max(dp[p][j-1], sum(p..i-1)), O(k n^2)
# follow-up: same shape -> ship packages within D days (LC 1011), Koko (LC 875)
