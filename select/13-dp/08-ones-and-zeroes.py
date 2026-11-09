# LC 474 Ones and Zeroes (Medium). Pattern: 0/1 knapsack with two capacities.
# strs are binary strings; pick the largest subset with at most m '0's and n '1's in
# total. Return the subset size.


# dp[i][j] = largest subset using at most i zeros and j ones. Each string is an item
# with weight (zeros, ones) and value 1; loop capacities downward so it is used once.
# T: O(L * m * n) for L strings, S: O(m * n).
def find_max_form(strs, m, n):
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    for word in strs:
        zeros = 0
        ones = 0
        for ch in word:
            if ch == "0":
                zeros += 1
            else:
                ones += 1
        # downward, like 1D 0/1 knapsack: dp[i - zeros][j - ones] is still "without word"
        for i in range(m, zeros - 1, -1):
            for j in range(n, ones - 1, -1):
                dp[i][j] = max(dp[i][j], dp[i - zeros][j - ones] + 1)
    return dp[m][n]


def test(label, strs, m, n, want):
    got = find_max_form(strs, m, n)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("pick-small-ones", ["0", "1", "01", "0011"], 2, 2, 3)
    test("zero-budget", ["0", "1"], 0, 1, 1)
    test("nothing-fits", ["00", "11"], 1, 1, 0)


main()

# follow-up: why loop downward -> upward would let dp[i - zeros][j - ones] already
#   include word, i.e. use the same string twice (unbounded knapsack)
# follow-up: 1D version of the same idea -> LC 416 partition equal subset sum
