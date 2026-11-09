# LC 76 Minimum Window Substring (Hard). Pattern: variable window, need counts +
# missing counter.
# Return the shortest substring of s containing every character of t with multiplicity
# (case-sensitive ASCII), or "" if none.


# Grow right until the window covers t, then shrink left past surplus chars.
# T: O(|s| + |t|), S: O(1) (128 slots).
def min_window(s, t):
    # need[code] = copies of that char the window still lacks (negative = extra copies)
    need = [0] * 128
    for c in t:
        need[ord(c)] += 1
    # chars of t the window still lacks; 0 = window covers t
    missing = len(t)
    best_start = 0
    best_len = -1
    left = 0
    for right in range(len(s)):
        code = ord(s[right])
        if need[code] > 0:
            missing -= 1
        need[code] -= 1
        if missing > 0:
            continue
        # drop left chars the window has more of than t needs
        while need[ord(s[left])] < 0:
            need[ord(s[left])] += 1
            left += 1
        window_len = right - left + 1
        if best_len == -1 or window_len < best_len:
            best_len = window_len
            best_start = left
    if best_len == -1:
        return ""
    return s[best_start:best_start + best_len]


def test(label, s, t, want):
    got = min_window(s, t)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("multiplicity", "abba", "aa", "abba")
    test("shorter-later", "xaybxab", "ab", "ab")
    test("whole", "cab", "abc", "cab")
    test("missing", "abc", "d", "")


main()

# follow-up: why O(n) -> right and left each move at most |s| times
# follow-up: t as subsequence (LC 727) -> scan forward to match t, then backward from
#   that end for the latest start; O(|s| * |t|)
