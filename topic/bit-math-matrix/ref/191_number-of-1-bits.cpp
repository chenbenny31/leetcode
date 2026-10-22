// LC 191 Number of 1 Bits: return the number of set bits in a 32-bit unsigned integer.
#include <iostream>
#include <string>
#include <cstdint>
using std::cout;
using std::string;
using std::uint32_t;

// n & (n - 1) clears the lowest set bit, so the loop runs once per set bit.
// T: O(number of set bits), S: O(1).
int hammingWeight(uint32_t n) {
    int cnt = 0;
    while (n != 0) {
        n &= n - 1;
        cnt++;
    }
    return cnt;
}

void test(const string& label, uint32_t n, int want) {
    cout << label << (hammingWeight(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", 11u, 3);
    test("zero", 0u, 0);
    test("top-bit", 0x80000000u, 1);
    test("all-ones", 0xffffffffu, 32);
}
