# LC 72 Edit Distance (Medium). Pattern: 2D DP over two prefixes.
# Return the min number of single-char inserts, deletes and replaces that turn word1
# into word2.


# Equal last chars cost nothing; else 1 + best of replace / delete / insert.
# T: O(m * n), S: O(m * n).
def min_distance(word1, word2):
    m = len(word1)
    n = len(word2)
    # dp[i][j] = edits to turn word1[0..i) into word2[0..j)
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    # to empty: delete all i chars; from empty: insert all j chars
    for i in range(m + 1):
        dp[i][0] = i
    for j in range(n + 1):
        dp[0][j] = j
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if word1[i - 1] == word2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1]
            else:
                # replace, delete word1[i-1], insert word2[j-1]
                dp[i][j] = 1 + min(dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1])
    return dp[m][n]


def test(label, word1, word2, want):
    got = min_distance(word1, word2)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("replace", "ab", "ac", 1)
    test("shift", "abc", "bcd", 2)
    test("classic", "kitten", "sitting", 3)
    test("empty", "", "ab", 2)


main()

# follow-up: O(n) space -> row i reads only row i-1: prev/cur rows, cur[0] = i,
#   prev, cur = cur, prev after each row; answer prev[n]
# follow-up: only insert / delete allowed -> m + n - 2 * LCS(word1, word2)
