// Apple Bit Operations (Easy) — bit masks, x & (x - 1)
// (1) Apply op "flip" or "clear" to bit i (from the LSB) of unsigned x and return it.
// (2) Count the set bits of a 32-bit unsigned value without strings or builtins.
// Clarify: i within 0..31; an unknown op returns x unchanged; value width (32 bits).

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

// Core idea: build a mask with only bit i set; flip = XOR with it, clear = AND with
// its inverse. T: O(1), S: O(1).
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

// Core idea: x & (x - 1) drops the lowest set bit, so the loop runs once per set bit.
// T: O(set bits), S: O(1).
int countBits(uint32_t x) {
    int count = 0;
    while (x != 0) {
        x &= x - 1;
        count++;
    }
    return count;
}

void test(const string& label, uint32_t got, uint32_t want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flip", applyOp(0b1010, 1, "flip"), 0b1000);
    test("clear", applyOp(0b1010, 3, "clear"), 0b0010);
    test("count", countBits(0b10110000), 3);
    test("count-all-ones", countBits(~0u), 32);
    return 0;
}

// follow-up (ideas only):
// - more ops -> set: OR with the mask; test: shift right by i and AND with 1
// - why 1u, not 1 -> shifting a signed 1 by 31 overflows int (undefined behavior)
// - count without the trick -> test each of the 32 bits with a shift and AND, O(32)
// - pow(x, n) (LC 50) -> 10-pow-x-n
// - count good numbers (LC 1922) -> 5^ceil(n/2) * 4^floor(n/2) mod 1e9+7 with modular
//   fast power (square the base per bit of the exponent, reduce after each product)
// - max k to sort a permutation (LC 3644) -> AND of every nums[i] with nums[i] != i;
//   0 if already sorted
