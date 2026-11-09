# LC 402 Remove K Digits (Medium). Pattern: monotonic increasing stack, greedy.
# num is a non-negative integer string; remove k digits to get the smallest number
# (no leading zeros, "0" if nothing is left).


# A bigger digit before a smaller one costs the most, so pop it while k lasts.
# T: O(n), S: O(n).
def remove_kdigits(num, k):
    # kept digits, non-decreasing (a char list: strings are immutable)
    stk = []
    for c in num:
        while k > 0 and stk and stk[-1] > c:
            stk.pop()
            k -= 1
        stk.append(c)
    # still non-decreasing: the largest digits sit at the end
    while k > 0 and stk:
        stk.pop()
        k -= 1
    start = 0
    while start < len(stk) and stk[start] == "0":
        start += 1
    result = "".join(stk[start:])
    if result == "":
        return "0"
    return result


def test(label, num, k, want):
    got = remove_kdigits(num, k)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("drop-peaks", "1532", 2, "12")
    test("leading-zeros", "3045", 1, "45")
    test("increasing", "1234", 2, "12")
    test("remove-all", "52", 2, "0")


main()

# follow-up: largest number instead -> same stack, pop while stk[-1] < c
# follow-up: remove duplicate letters (LC 316) -> same stack + last index of each char +
#   in_stack; pop the top only if it appears again later
