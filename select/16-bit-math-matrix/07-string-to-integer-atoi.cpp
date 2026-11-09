// LC 8 String to Integer (atoi) (Medium). Pattern: char scan, overflow check.
// Skip leading spaces, read an optional sign, then digits until a non-digit. Clamp to
// [INT_MIN, INT_MAX]; return 0 if there are no digits.

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

// follow-up: why INT_MIN is clamped too -> its magnitude 2147483648 doesn't fit in
//   result; returning INT_MIN on "overflow" with sign -1 is exactly right
