# LC 309 Best Time to Buy and Sell Stock with Cooldown (Medium). Pattern: state DP.
# prices[i] is the price on day i. Any number of trades, holding at most one share,
# and no buying on the day right after a sell. Return the max profit.


# Three states per day, each computed from yesterday's three. T: O(n), S: O(1).
def max_profit(prices):
    # best profit while holding a share; -inf = impossible before the first day
    hold = float("-inf")
    # best profit having sold today (tomorrow is the cooldown)
    sold = 0
    # best profit with no share and free to buy today
    rest = 0
    for price in prices:
        new_hold = max(hold, rest - price)
        new_sold = hold + price
        new_rest = max(rest, sold)
        hold = new_hold
        sold = new_sold
        rest = new_rest
    return max(sold, rest)


def test(label, prices, want):
    got = max_profit(prices)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("cooldown-bites", [2, 5, 0, 3], 3)
    test("rise", [1, 2, 3], 2)
    test("fall", [3, 2, 1], 0)
    test("empty", [], 0)


main()

# follow-up: transaction fee (LC 714) -> two states: hold = max(hold, free - price),
#   free = max(free, hold + price - fee)
# follow-up: at most k transactions (LC 188) -> hold[j] / free[j] per j <= k, O(n * k)
