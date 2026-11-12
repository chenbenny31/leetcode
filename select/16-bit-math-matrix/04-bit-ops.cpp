// Apple Bit Operations (Easy). Pattern: bit masks, x & (x - 1).
// (1) Given unsigned x, bit index i (from the LSB) and op "flip" or "clear", return x
// with op applied to bit i. (2) Count the set bits of a 32-bit unsigned value without
// strings or builtins.

#include <iostream>
#include <string>
#include <cstdint>
#include <climits>
#include <cmath>

using std::cout;
using std::string;
using std::uint32_t;
using std::fabs;

// flip = XOR with the mask, clear = AND with the inverted mask. T: O(1), S: O(1).
uint32_t applyOp(uint32_t x, int i, const string& op) {
    // only bit i set
    uint32_t mask = 1u << i;
    if (op == "flip") {
        return x ^ mask;
    } else if (op == "clear") {
        return x & ~mask;
    }
    return x;
}

// Test each of the 32 bits. T: O(32), S: O(1).
int countBits(uint32_t x) {
    int count = 0;
    for (int bit = 0; bit < 32; bit++) {
        count += (x >> bit) & 1;
    }
    return count;
}

// x & (x - 1) drops the lowest set bit: one loop per set bit. T: O(set bits), S: O(1).
int countBitsFast(uint32_t x) {
    int count = 0;
    while (x != 0) {
        x &= x - 1;
        count++;
    }
    return count;
}

// LC 50: walk the bits of n; base is squared each step and multiplied in on a set bit.
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

// base^exp % mod by squaring; products stay < mod^2 < 2^63. T: O(log exp), S: O(1).
long long powMod(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base % mod;
        }
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// LC 1922: even indices take an even digit (5 ways), odd indices a prime 2/3/5/7
// (4 ways): 5^ceil(n/2) * 4^floor(n/2) mod 1e9+7. T: O(log n), S: O(1).
int countGoodNumbers(long long n) {
    const long long MOD = 1000000007;
    long long evenWays = powMod(5, (n + 1) / 2, MOD);
    long long oddWays = powMod(4, n / 2, MOD);
    return (int)(evenWays * oddWays % MOD);
}

void test(const string& label, uint32_t got, uint32_t want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void testPow(const string& label, double x, int n, double want) {
    cout << label << (fabs(myPow(x, n) - want) < 1e-9 ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flip", applyOp(0b1010, 1, "flip"), 0b1000);
    test("clear", applyOp(0b1010, 3, "clear"), 0b0010);
    test("clear-zero-bit", applyOp(0b1010, 0, "clear"), 0b1010);
    test("count", countBits(0b10110000), 3);
    test("count-fast", countBitsFast(0b10110000), 3);
    test("all-ones", countBitsFast(~0u), 32);
    testPow("pow-square", 2.0, 10, 1024.0);
    testPow("pow-negative", 2.0, -2, 0.25);
    testPow("pow-zero-exp", 5.0, 0, 1.0);
    testPow("pow-int-min", -1.0, INT_MIN, 1.0);
    test("good-one", countGoodNumbers(1), 5);
    test("good-odd-len", countGoodNumbers(3), 100);
    test("good-mod", countGoodNumbers(30), 624001610);
}

// follow-up: more ops -> set: x | mask, test: (x >> i) & 1
// follow-up: why 1u, not 1 -> 1 << 31 overflows a signed int (undefined behavior)
// follow-up: max k to sort a permutation (LC 3644) -> AND of every nums[i] with
//   nums[i] != i; 0 if already sorted
