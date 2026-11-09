// LC 338 Counting Bits (Easy). Pattern: DP on i >> 1.
// For every i in [0, n], return the number of 1 bits in i.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// i has the bits of i / 2 plus its own lowest bit. T: O(n), S: O(n) for the output.
vector<int> countBits(int n) {
    // bits[i] = number of set bits in i
    vector<int> bits(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        bits[i] = bits[i >> 1] + (i & 1);
    }
    return bits;
}

void test(const string& label, int n, const vector<int>& want) {
    cout << label << (countBits(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, {0});
    test("five", 5, {0, 1, 1, 2, 1, 2});
    test("eight", 8, {0, 1, 1, 2, 1, 2, 2, 3, 1});
}

// follow-up: other recurrence -> bits[i] = bits[i & (i - 1)] + 1 (drops the lowest 1)
// follow-up: one number (LC 191) -> loop n &= n - 1 and count; runs once per set bit
