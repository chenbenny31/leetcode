# 09 · Minimum Remove to Make Valid Parentheses (ServiceNow intern 2026 phone, Medium)
# LC 1249: remove the fewest '(' or ')' so the str becomes valid; return any valid result
#
# clarify:
# other chars -> lowercase letters, kept as they are
# several valid results -> any is accepted; this one drops the unmatched ones
# empty str -> ""
# O(n) asked -> one pass with a stack, one pass to build


# core idea: match each ')' against a stack of open idxs; an unmatched ')' is dropped at
# once, the '(' left on the stack at the end are dropped too
# T: O(n), S: O(n)
def min_remove_to_make_valid(s):
    # str is immutable, so edit a list of chars; "" = removed
    chars = list(s)
    # idxs of '(' not matched yet
    open_idxs = []
    for i in range(len(chars)):
        if chars[i] == "(":
            open_idxs.append(i)
        elif chars[i] == ")":
            if open_idxs:
                open_idxs.pop()
            else:
                chars[i] = ""
    # '(' that never got closed
    for i in open_idxs:
        chars[i] = ""
    return "".join(chars)


def test(label, s, want):
    ok = min_remove_to_make_valid(s) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("extra-close", "(a))", "(a)")
    test("extra-open", "((a)", "(a)")
    test("all-bad", ")(", "")
    test("already-valid", "(a)b", "(a)b")


main()

# follow-up:
# 1. no stack -> balance cnt: left to right drop extra ')', right to left drop extra '('
# 2. all valid results with min removals (LC 301) -> BFS by num of removals, dedup set
# 3. only check validity (LC 20) -> stack of open brackets, match on each close
