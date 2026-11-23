# each mis-match cost one change, make both the min char, lexi-min
def make_smallest_palindrome(s):
    chars = list(s)
    ops = 0
    left = 0
    right = len(chars) - 1

    while left < right:
        if chars[left] != chars[right]:
            ops += 1
            c = min(chars[left], chars[right])
            chars[left] = c
            chars[right] = c
        left += 1
        right -= 1

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
