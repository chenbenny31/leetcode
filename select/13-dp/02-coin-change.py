# LC 322 Coin Change (Medium). Pattern: unbounded knapsack DP.
# Unlimited coins of each denomination; return the fewest coins that sum to amount,
# or -1 if no combination works.


# dp[sum] = 1 + min over coins of dp[sum - coin]. T: O(amount * coins), S: O(amount).
def coin_change(coins, amount):
    # dp[total] = fewest coins adding up to total; amount + 1 acts as infinity
    dp = [amount + 1] * (amount + 1)
    dp[0] = 0
    for total in range(1, amount + 1):
        for coin in coins:
            if coin <= total:
                dp[total] = min(dp[total], dp[total - coin] + 1)
    if dp[amount] > amount:
        return -1
    return dp[amount]


def test(label, coins, amount, want):
    got = coin_change(coins, amount)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("greedy-fails", [1, 3, 4], 6, 2)
    test("mixed", [2, 5], 11, 4)
    test("impossible", [5, 10], 3, -1)
    test("zero-amount", [1], 0, 0)


main()

# follow-up: number of combinations (LC 518) -> dp[total] += dp[total - coin], coins in
#   the OUTER loop, so each multiset is counted once (amounts outer counts orderings)
# follow-up: why not greedy -> [1, 3, 4], 6: greedy 4+1+1 = 3 coins, best 3+3 = 2
