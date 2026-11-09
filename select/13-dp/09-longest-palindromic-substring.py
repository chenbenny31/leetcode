# LC 5 Longest Palindromic Substring (Medium). Pattern: expand around center.
# Return the longest substring of s that reads the same both ways (any one if tied).


# grow s[left..right] outward while it stays a palindrome; returns the final length
def expand(s, left, right):
    n = len(s)
    while left >= 0 and right < n and s[left] == s[right]:
        left -= 1
        right += 1
    # the palindrome is s[left + 1..right - 1]
    return right - left - 1


# Try all 2n - 1 centers: each char (odd) and each gap (even). T: O(n^2), S: O(1).
def longest_palindrome(s):
    # best palindrome = s[start:start + best]
    start = 0
    best = 0
    n = len(s)
    for i in range(n):
        length = max(expand(s, i, i), expand(s, i, i + 1))
        if length > best:
            best = length
            # works for both: odd centers on i, even on the gap after i
            start = i - (length - 1) // 2
    return s[start:start + best]


def test(label, s, want):
    got = longest_palindrome(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("odd", "cabad", "aba")
    test("even", "xabbay", "abba")
    test("all-same", "aaaa", "aaaa")
    test("single", "z", "z")


main()

# follow-up: count palindromic substrings (LC 647) -> same 2n - 1 centers, +1 per
#   successful expand step, O(n^2)
# follow-up: O(n) -> Manacher: reuse the mirror's radius inside the rightmost palindrome
