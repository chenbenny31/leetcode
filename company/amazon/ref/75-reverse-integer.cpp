// LC 7: reverse the digits of a 32-bit signed integer; return 0 if the result overflows.
// Assume no 64-bit integers are available.

#include <iostream>
#include <string>
#include <climits>

using std::cout;
using std::string;

// Pop the last digit and push it onto the result, checking before each push that
// result * 10 + digit stays inside [INT_MIN, INT_MAX]. (In C++ x % 10 keeps x's sign.)
// T: O(digits), S: O(1).
int reverse(int x) {
    int result = 0;
    while (x != 0) {
        int digit = x % 10;
        x /= 10;
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) { return 0; }
        if (result < INT_MIN / 10 || (result == INT_MIN / 10 && digit < -8)) { return 0; }
        result = result * 10 + digit;
    }
    return result;
}

void test(const string& label, int x, int want) {
    cout << label << (reverse(x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("positive", 123, 321);
    test("trailing-zero", -120, -21);
    test("overflow", 1534236469, 0);
    test("min", INT_MIN, 0);
    test("zero", 0, 0);
}
