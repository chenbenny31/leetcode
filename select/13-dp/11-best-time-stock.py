# LC 121 Best Time to Buy and Sell Stock (Easy). Pattern: running min, one pass.
# prices[i] = price on day i; buy once, then sell on a later day. Return the max
# profit, 0 if no profit is possible.


# Sell today against the cheapest earlier day. T: O(n), S: O(1).
def max_profit(prices):
    # cheapest price seen so far
    min_price = prices[0]
    best = 0
    for price in prices:
        best = max(best, price - min_price)
        min_price = min(min_price, price)
    return best


def test(label, prices, want):
    got = max_profit(prices)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("basic", [3, 1, 4, 2], 3)
    test("late-min", [2, 5, 1, 3], 3)
    test("falling", [3, 2, 1], 0)
    test("one-day", [5], 0)


main()

# follow-up: unlimited transactions (LC 122) -> add every positive prices[i] -
#   prices[i - 1]
# follow-up: at most k transactions (LC 188) -> dp states buy[j] / sell[j] per j <= k,
#   O(n * k); cooldown variant is LC 309
