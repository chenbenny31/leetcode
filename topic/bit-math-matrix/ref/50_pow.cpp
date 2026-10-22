// LC 50 Pow(x, n): compute x raised to the integer power n (n may be negative).
#include <iostream>
#include <string>
#include <climits>
#include <cmath>
using std::cout;
using std::string;
using std::fabs;

// Fast exponentiation: square x and consume the bits of n. A negative n inverts x.
// T: O(log n), S: O(1).
double myPow(double x, int n) {
    long long e = n;   // 64 bits so that -INT_MIN does not overflow
    if (e < 0) {
        x = 1 / x;
        e = -e;
    }
    double res = 1;
    while (e > 0) {
        if (e & 1) { res *= x; }
        x *= x;
        e >>= 1;
    }
    return res;
}

void test(const string& label, double x, int n, double want) {
    cout << label << (fabs(myPow(x, n) - want) < 1e-9 ? " PASS\n" : " FAIL\n");
}

int main() {
    test("positive", 2.0, 10, 1024.0);
    test("negative-exponent", 2.0, -2, 0.25);
    test("zero-exponent", 5.0, 0, 1.0);
    test("int-min", 1.0, INT_MIN, 1.0);
}
