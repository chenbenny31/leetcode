# LC 312 Burst Balloons (Hard). Pattern: interval DP on the last element.
# Bursting balloon i earns left * nums[i] * right with its current neighbors (missing
# neighbor = 1); then those two become adjacent. Return the max total for bursting all.


# Pick the LAST balloon burst in (left, right): its neighbors are then exactly left and
# right. T: O(n^3), S: O(n^2).
def max_coins(nums):
    # nums padded with a 1 on both ends
    vals = [1]
    for num in nums:
        vals.append(num)
    vals.append(1)
    n = len(vals)

    # dp[left][right] = max coins from bursting every balloon strictly between them
    dp = [[0] * n for _ in range(n)]
    # shorter intervals first, so dp[left][last] and dp[last][right] are ready
    for gap in range(2, n):
        for left in range(n - gap):
            right = left + gap
            for last in range(left + 1, right):
                burst_last = vals[left] * vals[last] * vals[right]
                coins = dp[left][last] + burst_last + dp[last][right]
                dp[left][right] = max(dp[left][right], coins)
    return dp[0][n - 1]


def test(label, nums, want):
    got = max_coins(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("one", [5], 5)
    test("two", [2, 3], 9)
    test("three", [2, 3, 4], 36)
    test("zero-inside", [4, 0, 2], 12)


main()

# follow-up: why "last" not "first" -> if it bursts first its neighbors become adjacent
#   and the two halves interact; bursting it last keeps left and right as its neighbors
# follow-up: same shape -> min cost to cut a stick (LC 1547), matrix chain multiplication
