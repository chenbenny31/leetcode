// LC 190 Reverse Bits: reverse the bits of a 32-bit unsigned integer.
// Follow-up: the function is called many times; how would you optimize it?
#include <iostream>
#include <string>
#include <cstdint>
using std::cout;
using std::string;
using std::uint32_t;

// Pop the lowest bit of n and push it onto the low end of res, 32 times.
// T: O(32), S: O(1).
uint32_t reverseBits(uint32_t n) {
    uint32_t res = 0;
    for (int i = 0; i < 32; i++) {
        res = (res << 1) | (n & 1);
        n >>= 1;
    }
    return res;
}

// Follow-up: divide and conquer, swap halves of 16, 8, 4, 2, 1 bits with masks.
// T: O(log 32), S: O(1).
uint32_t reverseBitsSwap(uint32_t n) {
    n = (n >> 16) | (n << 16);
    n = ((n & 0xff00ff00u) >> 8) | ((n & 0x00ff00ffu) << 8);
    n = ((n & 0xf0f0f0f0u) >> 4) | ((n & 0x0f0f0f0fu) << 4);
    n = ((n & 0xccccccccu) >> 2) | ((n & 0x33333333u) << 2);
    n = ((n & 0xaaaaaaaau) >> 1) | ((n & 0x55555555u) << 1);
    return n;
}

void test(const string& label, uint32_t n, uint32_t want) {
    cout << label << (reverseBits(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("low-bit", 1u, 0x80000000u);
    test("pattern", 6u, 0x60000000u);
    test("all-ones", 0xffffffffu, 0xffffffffu);
    test("zero", 0u, 0u);
}
