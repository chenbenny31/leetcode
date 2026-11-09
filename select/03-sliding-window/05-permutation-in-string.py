# LC 567 Permutation in String (Medium). Pattern: fixed-size window of letter counts.
# Return true if some permutation of s1 is a substring of s2 (both lowercase).


# Slide a window of size |s1| over s2; a permutation iff its letter counts equal s1's.
# T: O(26 * n), S: O(1).
def check_inclusion(s1, s2):
    m = len(s1)
    n = len(s2)
    # need[c] = count of letter 'a' + c in s1
    need = [0] * 26
    for c in s1:
        need[ord(c) - ord("a")] += 1
    # window[c] = count of letter 'a' + c in s2[i-m+1..i]
    window = [0] * 26
    for i in range(n):
        window[ord(s2[i]) - ord("a")] += 1
        # s2[i - m] just left the size-m window
        if i >= m:
            window[ord(s2[i - m]) - ord("a")] -= 1
        if window == need:
            return True
    return False


def test(label, s1, s2, want):
    got = check_inclusion(s1, s2)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("middle", "ab", "xbay", True)
    test("at-end", "abc", "xxcab", True)
    test("count-differs", "aab", "abba", False)
    test("too-long", "abc", "ab", False)


main()

# follow-up: all start indices (LC 438) -> same loop, append i - m + 1 when equal
# follow-up: strict O(n) -> keep matches = letters whose counts agree; update it only
#   for the entering and leaving letter, permutation iff matches == 26
