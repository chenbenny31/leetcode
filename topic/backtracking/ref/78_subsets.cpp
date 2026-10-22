// LC 78 Subsets: given distinct integers, return every subset (the power set) in any
// order.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Bitmask enumeration: bit i of mask says whether nums[i] is in the subset.
// T: O(n * 2^n), S: O(1) beyond the output.
vector<vector<int>> subsets(vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> out;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> cur;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) { cur.push_back(nums[i]); }
        }
        out.push_back(cur);
    }
    return out;
}

void test(const string& label, vector<int> nums, vector<vector<int>> want) {
    vector<vector<int>> got = subsets(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3},
         {{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}});
    test("single", {5}, {{}, {5}});
    test("empty", {}, {{}});
}
