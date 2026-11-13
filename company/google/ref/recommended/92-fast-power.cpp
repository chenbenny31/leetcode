// 92 · Pow(x, n) (Fast Power) — LC 50, 1922
// Mother problem: select/16-bit-math-matrix/04-bit-ops.cpp
//
// LC 50 (Medium), fast power by squaring: return x^n for double x and any int n.
// Google reports: pow(x, n), count good numbers
//
// Clarify
// - n can be INT_MIN → negate in long long; negative n → 1 / x^|n|; n = 0 → 1.0
// - answer compared within 1e-9 (double)
//
// Core idea
// - Walk the bits of n: square the base each step, multiply it into the result on a set
//   bit. T O(log n), S O(1).
//
// Tests: 2^10, 2^-2, 5^0, (-1)^INT_MIN
//
// Follow-ups
// - modular power → same loop on long long with % mod after each product
//   (products stay < mod^2 < 2^63)
// - count good numbers (LC 1922) → 5^ceil(n/2) * 4^floor(n/2) mod 1e9+7 with powMod
// - bit ops (Apple, in the select file) → flip x ^ (1u << i), clear x & ~(1u << i),
//   count set bits with x &= x - 1
// - max k to sort a permutation (LC 3644) → AND of every nums[i] with nums[i] != i
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <climits>
#include <cmath>

using std::cout;
using std::string;
using std::fabs;

// Walk the bits of n; base is squared each step and multiplied in on a set bit.
// n as long long so -INT_MIN fits; negative n -> 1 / result. T: O(log n), S: O(1).
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
