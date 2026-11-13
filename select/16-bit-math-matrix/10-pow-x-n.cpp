// LC 50 Pow(x, n) (Medium) — exponentiation by squaring
// return x^n for integer n; n may be negative
//
// clarify:
// n = INT_MIN -> -n overflows int, use long long
// x = 0 w/ negative n -> 1 / 0 = inf, assume not asked
// overflow to infinity -> let double handle it
// precision -> compare w/ 1e-9

#include <iostream>
#include <string>
#include <cmath>
#include <climits>

using std::cout;
using std::string;
using std::fabs;

// core idea: read n in binary: square base per bit, multiply into result when bit set
// negative n = 1 / x^|n|
// T: O(log n), S: O(1)
double myPow(double x, int n) {
    // long long: -n overflows int when n = INT_MIN
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

// follow-up:
// modular pow, e.g. LC 1922 -> same loop on long long w/ % mod after every multiply
// n-th Fibonacci in O(log n) -> same squaring loop on 2 x 2 matrix {{1, 1}, {1, 0}}
// super pow, exponent as digit array (LC 372) -> per digit: res = res^10 * a^digit % 1337
// recursive form -> half = pow(x, n / 2); return half * half (* x when n is odd)
