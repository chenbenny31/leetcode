# 10 · Min-Change Palindrome, Smallest (phone screen, Easy) — LC 2697 + the count
# return (min replacements to make s a palindrome, the lexicographically smallest
# palindrome reachable with that many replacements)
#
# clarify:
# only replacements -> yes, no insert / delete
# chars -> lowercase letters
# odd len -> the middle char never changes
# empty str -> not given (len >= 1)


# core idea: each mismatched mirror pair costs exactly one change; make both the smaller
# char, which is also the lexicographically smallest choice
# T: O(n), S: O(n) — the list copy
def make_smallest_palindrome(s):
    # str is immutable, so edit a list of chars
    chars = list(s)
    ops = 0
    i = 0
    j = len(chars) - 1
    while i < j:
        if chars[i] != chars[j]:
            ops += 1
            c = min(chars[i], chars[j])
            chars[i] = c
            chars[j] = c
        i += 1
        j -= 1
    return ops, "".join(chars)


def test(label, s, want_ops, want):
    ops, result = make_smallest_palindrome(s)
    ok = ops == want_ops and result == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("one-pair", "ab", 1, "aa")
    test("two-pairs", "race", 2, "eaae")
    test("odd-middle", "zbx", 1, "xbx")
    test("already", "a", 0, "a")


main()

# follow-up:
# 1. at most k changes, k > mismatches -> spend the rest outside-in making "aa" pairs
# 2. insertions instead of replacements (LC 1312) -> n - longest palindromic subsequence
# 3. palindrome after one deletion (LC 680) -> on the 1st mismatch try both skips
