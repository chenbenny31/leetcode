# LC 20 Valid Parentheses (Easy). Pattern: stack of open brackets.
# s holds only ()[]{}. True if every bracket is closed by the same type, in order.


# Push openers; a closer must match the top. T: O(n), S: O(n).
def is_valid(s):
    # unmatched open brackets, innermost at the end
    stk = []
    for c in s:
        if c == "(" or c == "[" or c == "{":
            stk.append(c)
            continue
        opener = "{"
        if c == ")":
            opener = "("
        elif c == "]":
            opener = "["
        if not stk or stk[-1] != opener:
            return False
        stk.pop()
    # leftover openers were never closed
    return not stk


def test(label, s, want):
    got = is_valid(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("nested", "{[()]}", True)
    test("crossed", "{(})", False)
    test("unclosed", "((", False)
    test("extra-close", "())", False)


main()

# follow-up: only '(' ')' -> int depth instead of a stack, fail if it goes negative, O(1)
# follow-up: longest valid substring (LC 32) -> stack of indices seeded with -1; on ')'
#   pop, if empty push i else best = max(best, i - stk[-1])
