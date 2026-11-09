# LC 227 Basic Calculator II (Medium). Pattern: stack of signed terms.
# Evaluate a valid string of non-negative ints, + - * / and spaces; / truncates toward 0.
# No parentheses.


# Push each term with its sign; * and / apply to the last term at once. T: O(n), S: O(n).
def calculate(s):
    # signed terms whose sum is the answer
    terms = []
    # number being read
    num = 0
    # operator before num
    op = "+"
    n = len(s)
    for i in range(n):
        c = s[i]
        digit = "0" <= c <= "9"
        if digit:
            num = num * 10 + int(c)
        # apply the pending op at each operator, and once more at the end
        if (not digit and c != " ") or i == n - 1:
            if op == "+":
                terms.append(num)
            elif op == "-":
                terms.append(-num)
            elif op == "*":
                terms[-1] *= num
            else:
                # truncate toward 0: // floors, so -7 // 2 would give -4, not -3
                terms[-1] = int(terms[-1] / num)
            op = c
            num = 0
    return sum(terms)


# Follow-up: only the last term can still change, keep it instead of the stack.
# T: O(n), S: O(1).
def calculate_o1(s):
    # sum of finished terms
    result = 0
    # current signed term, still open to * and /
    last = 0
    num = 0
    op = "+"
    n = len(s)
    for i in range(n):
        c = s[i]
        digit = "0" <= c <= "9"
        if digit:
            num = num * 10 + int(c)
        if (not digit and c != " ") or i == n - 1:
            if op == "+" or op == "-":
                # + or - closes the last term
                result += last
                if op == "+":
                    last = num
                else:
                    last = -num
            elif op == "*":
                last *= num
            else:
                # truncate toward 0, not floor
                last = int(last / num)
            op = c
            num = 0
    return result + last


def test(label, s, want):
    ok = calculate(s) == want and calculate_o1(s) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("precedence", "2+3*4", 14)
    test("spaces", " 6 / 3 * 2 ", 4)
    test("minus-divide", "1-7/2", -2)
    test("multi-digit", "12-10/3+40", 49)


main()

# follow-up: parentheses (LC 772) -> on '(' recurse with a shared index i and use the
#   returned value as num; ')' ends the call like the end of the string
# follow-up: general precedence / unary minus -> shunting-yard with nums + ops stacks
