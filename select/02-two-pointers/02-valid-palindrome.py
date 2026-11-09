# LC 125 Valid Palindrome (Easy). Pattern: two pointers from both ends.
# After lowercasing and dropping every non-alphanumeric char, is s a palindrome?
# An empty result counts as a palindrome.


# Move inward, skipping non-alnum chars; compare lowercased. T: O(n), S: O(1).
def is_palindrome(s):
    left = 0
    right = len(s) - 1
    while left < right:
        if not s[left].isalnum():
            left += 1
        elif not s[right].isalnum():
            right -= 1
        elif s[left].lower() != s[right].lower():
            return False
        else:
            left += 1
            right -= 1
    return True


def test(label, s, want):
    got = is_palindrome(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("case-and-symbols", "No, on!", True)
    test("mismatch", "abca", False)
    test("digit-vs-letter", "2R", False)
    test("only-symbols", " ,.", True)


main()

# follow-up: delete at most one char (LC 680) -> at the first mismatch, return
#   is_range_palindrome(s, left + 1, right) or is_range_palindrome(s, left, right - 1);
#   each check is one more O(n) scan, so still O(n)
