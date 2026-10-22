// LC 260 Single Number III: exactly two elements appear once and all others twice;
// return the two singles in linear time and constant extra space.
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// XOR of all = a ^ b. Any set bit of it separates a from b, so XOR each group of
// numbers that has that bit set / clear. Unsigned avoids negating INT_MIN.
// T: O(n), S: O(1).
vector<int> singleNumber(vector<int>& nums) {
    unsigned both = 0;
    for (int x : nums) { both ^= unsigned(x); }
    unsigned low = both & (0u - both);   // lowest bit where a and b differ
    unsigned a = 0;
    unsigned b = 0;
    for (int x : nums) {
        if (unsigned(x) & low) {
            a ^= unsigned(x);
        } else {
            b ^= unsigned(x);
        }
    }
    return {int(a), int(b)};
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    vector<int> got = singleNumber(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {4, 1, 6, 4, 9, 6}, {1, 9});
    test("negative", {-2, 3, 3, 8, 8, 5}, {-2, 5});
    test("int-min", {INT_MIN, 0, 5, 5}, {INT_MIN, 0});
}
