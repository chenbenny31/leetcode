// 113 · String to Integer (atoi) — LC 8
// Mother: select/optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp
// parse s as int: skip leading spaces, one optional sign, digits until first non-digit
// no digits -> 0; clamp to [INT_MIN, INT_MAX]
// reported: LC tag #34
//
// clarify:
// which whitespace skipped -> only ' '
// sign -> at most one '+' / '-', right before the digits
// stop rule -> first non-digit ends the num
// 64-bit allowed? -> not needed, check before result * 10 + digit

#include <iostream>
#include <string>
#include <climits>

using std::cout;
using std::string;

// core idea: one pass: spaces, sign, digits; check overflow before result * 10 + digit
// on overflow clamp by sign (INT_MIN is reached via the clamp)
// T: O(n), S: O(1)
int myAtoi(const string& s) {
    int n = s.size();
    int i = 0;
    while (i < n && s[i] == ' ') {
        i++;
    }
    int sign = 1;
    if (i < n && (s[i] == '+' || s[i] == '-')) {
        if (s[i] == '-') {
            sign = -1;
        }
        i++;
    }
    // magnitude so far, always <= INT_MAX
    int result = 0;
    while (i < n && s[i] >= '0' && s[i] <= '9') {
        int digit = s[i] - '0';
        // result * 10 + digit would exceed INT_MAX
        if (result > (INT_MAX - digit) / 10) {
            return sign == 1 ? INT_MAX : INT_MIN;
        }
        result = result * 10 + digit;
        i++;
    }
    return sign * result;
}

void test(const string& label, const string& s, int want) {
    cout << label << (myAtoi(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spaces-sign-junk", "  -12ab", -12);
    test("no-digits", "a5", 0);
    test("overflow", "2147483648", INT_MAX);
    test("int-min", "-2147483648", INT_MIN);
}

// follow-up:
// why clamp INT_MIN too -> magnitude 2147483648 doesn't fit in result; clamp is exact
