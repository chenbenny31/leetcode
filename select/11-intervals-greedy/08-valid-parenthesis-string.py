# LC 678 Valid Parenthesis String (Medium). Pattern: greedy range of open counts.
# s has '(', ')' and '*'; each '*' may be '(', ')' or empty. Can s be a valid
# parenthesis string?


# Track the range [min_open, max_open] of possible open counts; valid iff it holds 0 at
# the end.
# T: O(n), S: O(1).
def check_valid_string(s):
    # fewest opens possible (each '*' as ')' while that keeps opens >= 0)
    min_open = 0
    # most opens possible (each '*' as '(')
    max_open = 0
    for c in s:
        if c == "(":
            min_open += 1
            max_open += 1
        elif c == ")":
            min_open -= 1
            max_open -= 1
        else:
            min_open -= 1
            max_open += 1
        # too many ')' even with every '*' as '('
        if max_open < 0:
            return False
        # a negative open count is never a real choice; clamp
        min_open = max(min_open, 0)
    return min_open == 0


def test(label, s, want):
    print(label, "PASS" if check_valid_string(s) == want else "FAIL")


def main():
    test("star-close", "((*)", True)
    test("star-empty", "(*)", True)
    test("close-first", "*)(", False)
    test("unclosed", "((*", False)


main()

# follow-up: why clamp min_open at 0 -> min_open < 0 means a '*' was used as ')' with
#   nothing open; treat that '*' as empty instead
# follow-up: index stacks -> stacks of '(' and '*' indices; ')' pops '(' first, else
#   '*'; at the end each leftover '(' needs a '*' at a later index. O(n) space
