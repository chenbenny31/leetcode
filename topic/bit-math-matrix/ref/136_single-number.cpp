// LC 136 Single Number: every element appears twice except one; return that one in
// linear time and constant extra space.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// XOR everything: equal pairs cancel (a ^ a = 0) and 0 ^ x = x. T: O(n), S: O(1).
int singleNumber(vector<int>& nums) {
    int res = 0;
    for (int x : nums) { res ^= x; }
    return res;
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (singleNumber(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {4, 9, 4}, 9);
    test("alone", {7}, 7);
    test("negative", {-3, 5, 5, 8, 8}, -3);
}
