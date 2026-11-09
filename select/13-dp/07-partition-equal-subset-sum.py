# LC 416 Partition Equal Subset Sum (Medium). Pattern: 0/1 knapsack, subset sum.
# nums are positive; can they be split into two subsets with equal sums?


# Some subset must reach total / 2; 1D bool DP over sums. T: O(n * sum), S: O(sum).
def can_partition(nums):
    total = 0
    for num in nums:
        total += num
    if total % 2 != 0:
        return False
    half = total // 2
    # dp[target] = some subset of the items seen so far adds up to target
    dp = [False] * (half + 1)
    dp[0] = True
    for num in nums:
        # high to low: dp[target - num] is still from before num, so num is used once
        for target in range(half, num - 1, -1):
            if dp[target - num]:
                dp[target] = True
    return dp[half]


def test(label, nums, want):
    got = can_partition(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("split", [3, 1, 1, 3], True)
    test("odd-total", [1, 2, 4], False)
    test("unreachable", [1, 1, 6], False)
    test("single", [2], False)


main()

# follow-up: why loop target downward -> upward would reuse num in the same pass, which
#   is the unbounded knapsack (coin change)
# follow-up: target sum (LC 494) -> count subsets summing to (total + target) // 2 with
#   dp[t] += dp[t - num], same downward loop
