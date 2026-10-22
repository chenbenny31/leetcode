// LC 300 Longest Increasing Subsequence: return the length of the longest strictly
// increasing subsequence of nums.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::lower_bound;
using std::string;
using std::vector;

// Patience piles: binary search the first pile top >= x and replace it.
// T: O(n log n), S: O(n).
int lengthOfLIS(const vector<int>& nums) {
    // tails[k] = smallest tail of any increasing subsequence of length k+1
    vector<int> tails;
    for (int x : nums) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return tails.size();
}

void test(const string& label, const vector<int>& nums, int want) {
    if (lengthOfLIS(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("mixed", {5, 1, 6, 2, 7}, 3);
    test("equal", {2, 2, 2}, 1);
    test("descending", {4, 3, 2, 1}, 1);
    test("empty", {}, 0);
}
