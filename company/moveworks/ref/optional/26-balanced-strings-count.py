# 26 · Number of Balanced Strings (Moveworks OA, Medium) — DP over the last letter
# count lowercase strs of len n where any two adjacent letters are at most d apart
# in the alphabet; answer mod 1e9+7. n = 2, d = 3 -> 170
#
# clarify:
# statement from a summary; its second example (n = 3, d = 3 -> 224) does not fit this
#   rule (the true count is far above 170), so confirm the statement
# mod -> assume 1e9+7
# n == 1 -> 26
# d >= 25 -> 26^n

MOD = 10**9 + 7


# core idea: ways[c] = strs of the cur len ending in letter c; the next letter c2
# takes the sum of ways[c] for c within d of c2
# T: O(n * 26 * d), S: O(26)
def count_balanced(n, d):
    # ways[c] = strs of the cur len ending in letter c; len 1 -> 1 each
    ways = [1] * 26
    for _ in range(n - 1):
        nxt = [0] * 26
        for c2 in range(26):
            for c in range(max(0, c2 - d), min(25, c2 + d) + 1):
                nxt[c2] = (nxt[c2] + ways[c]) % MOD
        ways = nxt
    return sum(ways) % MOD


def test(label, n, d, want):
    ok = count_balanced(n, d) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("neighbors-only", 2, 1, 76)
    test("one-letter", 1, 3, 26)
    test("same-letter-only", 5, 0, 26)
    test("any-pair", 2, 25, 676)


main()

# follow-up:
# 1. n up to 1e18 -> the step is a fixed 26 x 26 matrix: fast matrix power, O(26^3 log n)
# 2. faster step -> prefix sums over ways, O(26) per len instead of O(26 * d)
# 3. return one such str -> any constant str works ("aaa")
