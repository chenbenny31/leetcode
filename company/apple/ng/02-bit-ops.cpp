// Apple (hack2hire): (1) given unsigned x, bit index i (from the LSB) and an operation
// "flip" or "clear", return x with that operation applied to bit i, in one expression.
// (2) count the set bits of a 32-bit unsigned value without strings or builtins.
#include <iostream>
#include <string>
#include <cstdint>
using std::cout;
using std::string;
using std::uint32_t;

// flip = XOR with the mask, clear = AND with the inverted mask; one branch per op
// so a new op ("set", "test") is one more branch. T: O(1), S: O(1).
uint32_t applyOp(uint32_t x, int i, const string& op) {
    uint32_t mask = 1u << i;   // only bit i set
    if (op == "flip") {
        return x ^ mask;
    } else if (op == "clear") {
        return x & ~mask;
    }
    return x;   // unknown op: leave x unchanged
}

// straightforward: test each of the 32 bits. T: O(32), S: O(1).
int countBits(uint32_t x) {
    int cnt = 0;
    for (int i = 0; i < 32; i++) {
        cnt += (x >> i) & 1;
    }
    return cnt;
}

// improved: x & (x - 1) drops the lowest set bit, so loop once per set bit.
// T: O(set bits) <= 32, S: O(1).
int countBitsFast(uint32_t x) {
    int cnt = 0;
    while (x != 0) {
        x &= x - 1;
        cnt++;
    }
    return cnt;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    check(applyOp(0b1010, 1, "flip") == 0b1000, "flip");
    check(applyOp(0b1010, 3, "clear") == 0b0010, "clear");
    check(countBits(0b10110000) == 3 && countBitsFast(0b10110000) == 3, "count");
    check(countBits(~0u) == 32 && countBitsFast(~0u) == 32, "all-ones");
}
