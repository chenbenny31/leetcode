# LC 3 Longest Substring Without Repeating Characters (Medium). Pattern: variable
# window, jump left past last seen index.
# Return the length of the longest substring of s (ASCII) with no repeated character.


# Window s[left..right] has no repeats; on a repeat, jump left past that char's last
# index. T: O(n), S: O(1) (128 slots).
def length_of_longest_substring(s):
    # last[code] = latest index of the char with that ASCII code, -1 = not seen yet
    last = [-1] * 128
    best = 0
    left = 0
    for right in range(len(s)):
        code = ord(s[right])
        # max: a last index left of the window must not move left backwards
        left = max(left, last[code] + 1)
        last[code] = right
        best = max(best, right - left + 1)
    return best


def test(label, s, want):
    got = length_of_longest_substring(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("repeat-at-start", "abcad", 4)
    test("all-same", "aaa", 1)
    test("stale-index", "abba", 2)
    test("empty", "", 0)


main()

# follow-up: return the substring -> record best_start = left whenever best improves
# follow-up: at most k distinct chars (LC 340) -> count[c] + distinct; shrink left one
#   step at a time while distinct > k (can't jump), O(n)
# follow-up: large alphabet / ints -> dict last, same logic
