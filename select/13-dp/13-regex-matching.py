# LC 10 Regular Expression Matching (Hard). Pattern: 2D DP over two prefixes.
# Does pattern p match all of s? '.' matches any one char; '*' matches zero or more
# of the element right before it.


# "x*" either matches nothing or eats one more char of s. T: O(m * n), S: O(m * n).
def is_match(s, p):
    m = len(s)
    n = len(p)
    # dp[i][j] = s[0..i) matches p[0..j)
    dp = [[False] * (n + 1) for _ in range(m + 1)]
    dp[0][0] = True
    # i starts at 0: "a*" can match the empty prefix
    for i in range(m + 1):
        for j in range(1, n + 1):
            if p[j - 1] == "*":
                # the x in "x*"
                starred = p[j - 2]
                # zero copies: drop "x*"
                zero = dp[i][j - 2]
                # one more copy: s[i-1] matches x, and s[0..i-1) still matches "..x*"
                char_match = i > 0 and (starred == "." or starred == s[i - 1])
                more = char_match and dp[i - 1][j]
                dp[i][j] = zero or more
            else:
                pat_char = p[j - 1]
                char_match = i > 0 and (pat_char == "." or pat_char == s[i - 1])
                dp[i][j] = char_match and dp[i - 1][j - 1]
    return dp[m][n]


def test(label, s, p, want):
    got = is_match(s, p)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("star-zero", "b", "a*b", True)
    test("star-many", "aaab", "a*b", True)
    test("dot-star", "abc", "a.*", True)
    test("tail-mismatch", "ab", ".*c", False)


main()

# follow-up: wildcard (LC 44, '?' one char, '*' any sequence) -> '*': dp[i][j - 1] or
#   dp[i - 1][j]; '?' like '.', O(m * n)
# follow-up: O(n) space -> row i reads only rows i and i - 1: prev/cur lists of bool
