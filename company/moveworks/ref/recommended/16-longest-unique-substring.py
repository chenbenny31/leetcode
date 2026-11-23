# 16 · Longest Substring Without Repeats, Return It (pasted batch, Medium) — LC 3
# return the longest substring of s with no repeated char (not just its len)
#
# clarify:
# several of the max len -> return the first one (any is accepted)
# chars -> any; a dict handles every char
# empty str -> ""
# len -> up to 5e4, so O(n)


# core idea: window s[left:right + 1] has no repeats; when s[right] was seen inside the
# window, jump left past its last idx; remember where the best window starts
# T: O(n), S: O(num of distinct chars)
def longest_unique_substring(s):
    # last[c] = last idx where char c was seen
    last = {}
    left = 0
    best_start = 0
    best_len = 0
    for right in range(len(s)):
        c = s[right]
        # only a repeat inside the window moves left; an older one is ignored
        if c in last and last[c] >= left:
            left = last[c] + 1
        last[c] = right
        if right - left + 1 > best_len:
            best_len = right - left + 1
            best_start = left
    return s[best_start:best_start + best_len]


def test(label, s, want):
    ok = longest_unique_substring(s) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("first-of-ties", "abcab", "abc")
    test("later-start", "aab", "ab")
    test("stale-idx", "abba", "ab")
    test("empty", "", "")


main()

# follow-up:
# 1. only the len (LC 3) -> return best_len
# 2. at most k distinct (LC 340) -> cnt dict of the window, shrink while len(cnt) > k
# 3. at most 2 of each char -> cnt dict, shrink while cnt[c] > 2
