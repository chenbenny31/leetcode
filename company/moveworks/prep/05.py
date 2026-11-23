# match each ) against a stack of open idxs, drop ) in-flight and ( after scanning
def min_remove_to_make_valid(s):
    chars = list(s)
    open_idxs = []  # indices of '(' not matched yet

    for i in range(len(chars)):
        if chars[i] == "(":
            open_idxs.append(i)
        elif chars[i] == ")":
            if open_idxs:
                open_idxs.pop()
            else:
                chars[i] = ""  # del in-flight unmatched )

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
