// LC 29 Divide Two Integers: divide two 32-bit ints without *, / or %, truncating
// toward zero. Return INT_MAX if the quotient overflows.
#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
using std::cout;
using std::string;
using std::llabs;

// Shift-subtract: from the top bit down, peel off b << i whenever it still fits.
// T: O(32), S: O(1).
int divide(int dividend, int divisor) {
    if (dividend == INT_MIN && divisor == -1) { return INT_MAX; }
    bool neg = (dividend < 0) != (divisor < 0);
    // magnitudes in 64 bits so that INT_MIN is safe
    long long a = llabs(dividend);
    long long b = llabs(divisor);
    long long quo = 0;
    for (int i = 31; i >= 0; i--) {
        if ((a >> i) >= b) {
            a -= b << i;
            quo += 1LL << i;
        }
    }
    return neg ? -quo : quo;
}

void test(const string& label, int dividend, int divisor, int want) {
    cout << label << (divide(dividend, divisor) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("negative-truncates", 7, -2, -3);
    test("overflow", INT_MIN, -1, INT_MAX);
    test("int-min", INT_MIN, 1, INT_MIN);
    test("zero", 0, 5, 0);
}
