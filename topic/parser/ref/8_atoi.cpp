// LC 8 String to Integer (atoi): skip leading spaces, read an optional sign, then read
// digits until a non-digit. No digits gives 0. Clamp the result to the 32-bit int range.
#include <iostream>
#include <string>
#include <climits>
using std::cout;
using std::string;

// One left-to-right pass with a long long accumulator, clamped as soon as it overflows.
// T: O(n), S: O(1).
int myAtoi(string s) {
    int n = s.size(), i = 0, sign = 1;
    long long val = 0;   // magnitude so far, never above ~2^31 * 10
    while (i < n && s[i] == ' ') {
        i++;
    }
    if (i < n && (s[i] == '+' || s[i] == '-')) {
        sign = s[i] == '-' ? -1 : 1;
        i++;
    }
    while (i < n && s[i] >= '0' && s[i] <= '9') {
        val = val * 10 + (s[i] - '0');
        // clamp inside the loop so val can never overflow long long
        if (sign * val > INT_MAX) { return INT_MAX; }
        if (sign * val < INT_MIN) { return INT_MIN; }
        i++;
    }
    return sign * val;
}

void test(const string& label, const string& s, int want) {
    cout << label << (myAtoi(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sign-and-tail", "   -42abc", -42);
    test("overflow", "99999999999", INT_MAX);
    test("underflow", "-99999999999", INT_MIN);
    test("no-digits", "abc 12", 0);
}
