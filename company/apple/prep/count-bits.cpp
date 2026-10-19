// given unsigned x, bit index i (from LSB) with op "flip" or "clear"
// return x with op applied onto bit i
// count the set bits of a 32-bit unsigned value without strings or builtins

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

// flip = XOR with mask, clear = AND with inverted maks
uint32_t applyOp(uint32_t x, int i, const string& op) {
    if (op == "flip") {
        return x ^ (1u << i);
    } else if (op == "clear") {
        return x & ~(1u << i);
    }
    return x;
}

// test each of 32 bits, T: O(32), S: O(1)
int countBits(uint32_t x) {
    int cnt = 0;
    for (int i = 0; i < 32; i++) {
        cnt += (x >> i) & 1;
    }
    return cnt;
}

// x & (x - 1) drops the LSB, loop until x is 0, T: O(32), S: O(1)
int countBitsFast(uint32_t x) {
    int cnt = 0;
    while (x != 0) {
        x &= x - 1;
        cnt++;
    }
    return cnt;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " pass\n" : " fail\n");
}

int main() {
    check(applyOp(0b1010, 1, "flip") == 0b1000, "flip");
    check(applyOp(0b1010, 3, "clear") == 0b0010, "clear");
    check(countBits(0b10110000) == 3, "count");
    check(countBits(0) == 0 && countBits(0xFFFFFFFFu) == 32, "count edges");
}
