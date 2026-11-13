// 108 · String to Integer (atoi) — LC 8
// All variants with code:
//   select/optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp
//
// Google reports: LC tag #34
//
// Clarify
// - whitespace? → skip leading ' ' only
// - sign? → one optional '+' or '−'
// - stop rule? → first non-digit ends the number; no digits → 0
// - overflow? → clamp to [INT_MIN, INT_MAX]; no 64-bit allowed in the strict version
//
// Core idea
// - One pass: spaces, sign, then digits.
// - Before result = result · 10 + digit, check result > (INT_MAX − digit) / 10 and clamp
//   by sign.
// - INT_MIN is reached through the clamp, since its magnitude doesn't fit in int.
// - T O(n), S O(1).
//
// Tests: spaces sign junk, no digits, overflow, int min
//
// Follow-ups
// - why INT_MIN is clamped too → magnitude 2147483648 overflows result; returning
//   INT_MIN on overflow with sign −1 is exactly right
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <climits>

using std::cout;
using std::string;

// One pass; check overflow before result * 10 + digit. T: O(n), S: O(1).
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
