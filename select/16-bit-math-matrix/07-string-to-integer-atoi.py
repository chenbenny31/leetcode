# LC 8 String to Integer (atoi) (Medium). Pattern: char scan, overflow check.
# Skip leading spaces, read an optional sign, then digits until a non-digit. Clamp to
# [INT_MIN, INT_MAX]; return 0 if there are no digits.

# 32-bit bounds written out: Python ints never overflow, so the clamp is explicit
INT_MAX = 2**31 - 1
INT_MIN = -(2**31)


# One pass; check overflow before result * 10 + digit. T: O(n), S: O(1).
def my_atoi(s):
    n = len(s)
    i = 0
    while i < n and s[i] == " ":
        i += 1
    sign = 1
    if i < n and (s[i] == "+" or s[i] == "-"):
        if s[i] == "-":
            sign = -1
        i += 1
    # magnitude so far, always <= INT_MAX
    result = 0
    while i < n and s[i] >= "0" and s[i] <= "9":
        digit = ord(s[i]) - ord("0")
        # result * 10 + digit would exceed INT_MAX (both sides >= 0, so // is safe)
        if result > (INT_MAX - digit) // 10:
            if sign == 1:
                return INT_MAX
            return INT_MIN
        result = result * 10 + digit
        i += 1
    return sign * result


def test(label, s, want):
    got = my_atoi(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("spaces-sign-junk", "  -12ab", -12)
    test("no-digits", "a5", 0)
    test("overflow", "2147483648", INT_MAX)
    test("int-min", "-2147483648", INT_MIN)


main()

# follow-up: why INT_MIN is clamped too -> its magnitude 2147483648 doesn't fit in
#   result; returning INT_MIN on "overflow" with sign -1 is exactly right
# follow-up: Python shortcut -> ints don't overflow, so one could read all digits and
#   clamp once at the end, but a 10^5-digit input then builds a huge int; the early
#   check stops at the 11th digit
