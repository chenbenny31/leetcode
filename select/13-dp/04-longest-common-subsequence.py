# LC 1143 Longest Common Subsequence (Medium). Pattern: 2D DP over two prefixes.
# Return the length of the longest subsequence common to text1 and text2 (0 if none).


# Match last chars -> diagonal + 1, else drop one char from either side.
# T: O(m * n), S: O(m * n).
def longest_common_subsequence(text1, text2):
    m = len(text1)
    n = len(text2)
    # dp[i][j] = LCS length of text1[0..i) and text2[0..j)
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if text1[i - 1] == text2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])
    return dp[m][n]


def test(label, text1, text2, want):
    got = longest_common_subsequence(text1, text2)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("skip-chars", "axbycz", "abc", 3)
    test("crossing", "abcd", "acbd", 3)
    test("disjoint", "abc", "def", 0)
    test("empty", "", "abc", 0)


main()

# follow-up: O(min(m, n)) space -> two rows prev/cur over the shorter string
# follow-up: return the LCS string -> backtrack from dp[m][n]: equal chars -> take,
#   i -= 1, j -= 1; else step toward the larger of dp[i-1][j], dp[i][j-1]; collect chars
#   in a list, reverse, "".join at the end
