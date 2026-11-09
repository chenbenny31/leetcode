# LC 198 House Robber (Medium). Pattern: 1D DP, take or skip.
# Houses in a row hold nums[i] >= 0; return the max total you can rob without robbing
# two adjacent houses.


# dp[i] = max(dp[i-1], dp[i-2] + nums[i]) in two rolling vars. T: O(n), S: O(1).
def rob(nums):
    # best loot up to the house before the previous one
    prev = 0
    # best loot up to the previous house
    cur = 0
    for money in nums:
        # skip this house, or rob it on top of prev
        best = max(cur, prev + money)
        prev = cur
        cur = best
    return cur


def test(label, nums, want):
    got = rob(nums)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("skip-two", [2, 1, 1, 2], 4)
    test("middle-big", [1, 3, 1], 3)
    test("mixed", [3, 10, 3, 1, 2], 12)
    test("single", [4], 4)


main()

# follow-up: houses in a circle (LC 213) -> first and last can't both be robbed:
#   max(rob(nums[:-1]), rob(nums[1:])); n == 1 -> nums[0]
# follow-up: binary tree (LC 337) -> dfs returns (take, skip): take = val + skip(L) +
#   skip(R), skip = max(L) + max(R); O(n)
