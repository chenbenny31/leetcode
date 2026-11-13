// Apple Bit Operations (Easy) — bit masks, x & (x - 1)
// (1) apply op "flip" or "clear" to bit i (from LSB) of unsigned x, return it
// (2) cnt set bits of a 32-bit unsigned value w/o strings or builtins
//
// clarify:
// i range -> assume 0..31
// unknown op -> return x unchanged
// value width -> 32 bits
// more ops (set / test)? -> follow-up

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

// (1) flip / clear bit i
// core idea: mask w/ only bit i set; flip = XOR mask, clear = AND ~mask
// T: O(1), S: O(1)
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

// (2) count set bits
// core idea: x & (x - 1) drops lowest set bit -> loop runs once per set bit
// T: O(set bits), S: O(1)
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

// follow-up:
// more ops -> set: OR w/ mask; test: (x >> i) & 1
// why 1u, not 1 -> signed 1 << 31 overflows int (undefined behavior)
// cnt w/o the trick -> test each of 32 bits w/ shift + AND, O(32)
// pow(x, n) (LC 50) -> 10-pow-x-n
// count good numbers (LC 1922) -> 5^ceil(n/2) * 4^floor(n/2) mod 1e9+7, modular fast pow
// max k to sort a permutation (LC 3644) -> AND of all nums[i] != i; 0 if sorted
