// Apple Bit Operations (Easy). Pattern: bit masks, x & (x - 1).
// (1) Given unsigned x, bit index i (from the LSB) and op "flip" or "clear", return x
// with op applied to bit i. (2) Count the set bits of a 32-bit unsigned value without
// strings or builtins.

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

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

void test(const string& label, uint32_t got, uint32_t want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flip", applyOp(0b1010, 1, "flip"), 0b1000);
    test("clear", applyOp(0b1010, 3, "clear"), 0b0010);
    test("clear-zero-bit", applyOp(0b1010, 0, "clear"), 0b1010);
    test("count", countBits(0b10110000), 3);
    test("count-fast", countBitsFast(0b10110000), 3);
    test("all-ones", countBitsFast(~0u), 32);
}

// follow-up: more ops -> set: x | mask, test: (x >> i) & 1
// follow-up: why 1u, not 1 -> 1 << 31 overflows a signed int (undefined behavior)
