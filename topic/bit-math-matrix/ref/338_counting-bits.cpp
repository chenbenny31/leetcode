// LC 338 Counting Bits: for every i in [0, n], return the number of 1 bits in i.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// DP on the shifted value: bits[i] = bits[i / 2] + the lowest bit of i.
// T: O(n), S: O(n) for the output.
vector<int> countBits(int n) {
    vector<int> bits(n + 1, 0);   // bits[i] = number of set bits in i
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
    test("eight", 8, {0, 1, 1, 2, 1, 2, 2, 3, 1});
}
