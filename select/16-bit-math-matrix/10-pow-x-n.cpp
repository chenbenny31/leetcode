// LC 50 Pow(x, n) (Medium) — exponentiation by squaring
// Return x raised to the integer power n; n may be negative.
// Clarify: n = INT_MIN; x = 0 with a negative n; overflow to infinity; precision needed.

#include <iostream>
#include <string>
#include <cmath>
#include <climits>

using std::cout;
using std::string;
using std::fabs;

// Core idea: read n in binary: square the base once per bit and multiply it into the
// result when the bit is set; a negative n is 1 / x^|n|. T: O(log n), S: O(1).
double myPow(double x, int n) {
    // long long: -n overflows int when n is INT_MIN
    long long exponent = n;
    if (exponent < 0) { exponent = -exponent; }
    double result = 1.0;
    // base = x^(2^bit) for the bit being read
    double base = x;
    while (exponent > 0) {
        if (exponent % 2 == 1) { result *= base; }
        base *= base;
        exponent /= 2;
    }
    if (n < 0) { return 1.0 / result; }
    return result;
}

void test(const string& label, double x, int n, double want) {
    bool ok = fabs(myPow(x, n) - want) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-exponent", 3.0, 5, 243.0);
    test("negative-exponent", 2.0, -2, 0.25);
    test("zero-exponent", 7.0, 0, 1.0);
    test("int-min-exponent", -1.0, INT_MIN, 1.0);
    return 0;
}

// follow-up (ideas only):
// - modular power, e.g. count good numbers (LC 1922) -> the same loop on long long with
//   % mod after every multiplication (mod^2 must fit in long long)
// - n-th Fibonacci in O(log n) -> the same squaring loop on the 2 x 2 matrix
//   {{1, 1}, {1, 0}}; its n-th power holds F(n)
// - super pow, exponent given as a digit array (LC 372) -> per digit:
//   result = result^10 * a^digit, all mod 1337
// - recursive form -> half = pow(x, n / 2); return half * half (* x when n is odd)
