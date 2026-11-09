# LC 424 Longest Repeating Character Replacement (Medium). Pattern: variable window,
# len - max_freq <= k.
# s of uppercase letters; change at most k chars to any letter. Return the length of
# the longest substring of one repeated letter.


# Window is valid while its non-majority chars (len - max_freq) fit in k.
# T: O(n), S: O(1).
def character_replacement(s, k):
    # count[c] = count of letter 'A' + c in the window s[left..right]
    count = [0] * 26
    # highest single-letter count any window has reached
    max_freq = 0
    best = 0
    left = 0
    for right in range(len(s)):
        letter = ord(s[right]) - ord("A")
        count[letter] += 1
        max_freq = max(max_freq, count[letter])
        # max_freq is never decreased: only a larger max_freq can beat best
        while right - left + 1 - max_freq > k:
            count[ord(s[left]) - ord("A")] -= 1
            left += 1
        best = max(best, right - left + 1)
    return best


def test(label, s, k, want):
    got = character_replacement(s, k)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("no-change", "AABA", 0, 2)
    test("one-change", "ABAA", 1, 4)
    test("spread", "ABCAB", 2, 4)
    test("single", "B", 0, 1)


main()

# follow-up: why a stale max_freq is fine -> it only keeps an old valid window length;
#   best grows only when a real larger max_freq appears
# follow-up: binary array, flip <= k zeros (LC 1004) -> count zeros in window, shrink
#   left while zeros > k, O(n)
