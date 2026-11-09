# LC 224 Basic Calculator (Hard). Pattern: stack of (result, sign) at each '('.
# Evaluate a valid expression of non-negative ints, + - ( ) and spaces; '-' may be unary,
# e.g. "-(2+3)". No eval().


# Only + and -, so keep a running sum; at '(' save the outer sum and the sign before the
# paren, at ')' fold the inner sum back in. T: O(n), S: O(n).
def calculate(s):
    # sum of finished terms at the current depth
    result = 0
    # number being read
    num = 0
    # sign before num
    sign = 1
    # saved[d] = (outer result, sign before '(') for each open paren
    saved = []
    for c in s:
        if "0" <= c <= "9":
            num = num * 10 + int(c)
        elif c == "+" or c == "-":
            result += sign * num
            num = 0
            sign = 1 if c == "+" else -1
        elif c == "(":
            saved.append((result, sign))
            result = 0
            sign = 1
        elif c == ")":
            result += sign * num
            num = 0
            outer, outer_sign = saved.pop()
            # the paren's value becomes one signed term of the outer sum
            result = outer + outer_sign * result
    return result + sign * num


def test(label, s, want):
    got = calculate(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("spaces", " 7 - 3 + 1 ", 5)
    test("minus-paren", "5-(2-3)", 6)
    test("nested", "2-(3-(4+1))", 4)
    test("unary-minus", "-(2+3)", -5)


main()

# follow-up: add * and / (LC 772) -> recursion: calc(s, i) runs the LC 227 loop until
#   ')' or the end; '(' recurses and its return value becomes num
