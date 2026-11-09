// LC 136 Single Number (Easy). Pattern: XOR cancels pairs.
// Every element appears twice except one; return that one in O(n) time and O(1)
// extra space.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// XOR everything: a ^ a = 0 and 0 ^ x = x, order doesn't matter. T: O(n), S: O(1).
int singleNumber(const vector<int>& nums) {
    int single = 0;
    for (int num : nums) {
        single ^= num;
    }
    return single;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {4, 9, 4}, 9);
    test("alone", {7}, 7);
    test("negative", {-3, 5, 5, 8, 8}, -3);
}

// follow-up: others appear three times (LC 137) -> per bit count set bits mod 3; the
//   leftover bits form the answer, O(32n)
// follow-up: two singles (LC 260) -> x = xor of all; split nums by the lowest set bit
//   of x (unsigned d = x & -(unsigned)x) and xor each group
