// 97 · Pow(x, n) (Fast Power) — LC 50, 1922
// Mother: select/16-bit-math-matrix/04-bit-ops.cpp
// return x^n for a double x and any int n
// reported: pow(x, n), count good numbers
//
// clarify:
// negative n -> 1 / x^|n|
// n = INT_MIN -> -n overflows int, use long long
// n = 0 -> 1
// x = 0 w/ negative n -> inf here; assume not given
// precision expected -> 1e-9 tolerance

#include <iostream>
#include <string>
#include <climits>
#include <cmath>

using std::cout;
using std::string;
using std::fabs;

// core idea: walk the bits of n; square base each step, multiply into result on a set bit
// n in a long long so -INT_MIN fits; negative n -> 1 / result
// T: O(log n), S: O(1)
double myPow(double x, int n) {
    long long power = n;
    bool negative = power < 0;
    if (negative) {
        power = -power;
    }
    double result = 1.0;
    // x^(2^b) for the current bit b of power
    double base = x;
    while (power > 0) {
        if (power & 1) {
            result *= base;
        }
        base *= base;
        power >>= 1;
    }
    return negative ? 1.0 / result : result;
}

void test(const string& label, double x, int n, double want) {
    cout << label << (fabs(myPow(x, n) - want) < 1e-9 ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", 2.0, 10, 1024.0);
    test("negative", 2.0, -2, 0.25);
    test("zero-exp", 5.0, 0, 1.0);
    test("int-min", -1.0, INT_MIN, 1.0);
}

// follow-up:
// modular power -> same loop on long long w/ % mod after each product (mod^2 < 2^63)
// count good numbers (LC 1922, Google) -> 5^ceil(n/2) * 4^floor(n/2) mod 1e9+7
// bit ops (Apple, in the mother file) -> flip = XOR w/ one-bit mask, clear = AND w/ ~mask
// cnt set bits -> clear the lowest one each step
// max k to sort a permutation (LC 3644) -> AND of every nums[i] w/ nums[i] != i
