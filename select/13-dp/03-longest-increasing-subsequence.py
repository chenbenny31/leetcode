# LC 300 Longest Increasing Subsequence (Medium). Pattern: 1D DP; patience tails.
# Return the length of the longest strictly increasing subsequence of nums.


# dp[i] = 1 + max dp[j] over j < i with nums[j] < nums[i]. T: O(n^2), S: O(n).
def length_of_lis(nums):
    n = len(nums)
    # dp[i] = LIS length ending at nums[i]
    dp = [1] * n
    best = 0
    for i in range(n):
        for j in range(i):
            if nums[j] < nums[i]:
                dp[i] = max(dp[i], dp[j] + 1)
        best = max(best, dp[i])
    return best


# Follow-up: keep the smallest tail per length, binary search where num goes.
# T: O(n log n), S: O(n).
def length_of_lis_tails(nums):
    # tails[k] = smallest tail of any increasing subsequence of length k + 1
    tails = []
    for num in nums:
        # first tail >= num (>= keeps it strictly increasing)
        lo = 0
        hi = len(tails)
        while lo < hi:
            mid = (lo + hi) // 2
            if tails[mid] < num:
                lo = mid + 1
            else:
                hi = mid
        if lo == len(tails):
            tails.append(num)
        else:
            tails[lo] = num
    return len(tails)


def test(label, nums, want):
    ok = length_of_lis(nums) == want and length_of_lis_tails(nums) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("replace-tail", [3, 1, 2, 4], 3)
    test("zigzag", [1, 5, 2, 6, 3, 7], 4)
    test("dups-strict", [2, 2, 3, 3], 2)
    test("decreasing", [3, 2, 1], 1)


main()

# follow-up: print one LIS -> parent[i] = the j that set dp[i]; walk parents back from
#   the argmax of dp, then reverse
# follow-up: non-decreasing -> first tail > num instead of >= num (bisect_right)
# follow-up: envelopes (LC 354) -> sort w asc, h desc on ties, then LIS on h
# follow-up: the hand-written loop is bisect.bisect_left(tails, num)
