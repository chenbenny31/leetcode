// Treat nums as a set of distinct values. Return the length of the longest chain of
// values a1 < a2 < ... < aL from nums with a(i+1) - a(i) < k for every adjacent pair.
// (k = 2 is LC 128, Longest Consecutive Sequence.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::unique;
using std::max;

// After sorting the distinct values, a chain is a run whose neighboring gaps are all
// < k; any skipped value in between would only shrink a gap. T: O(n log n), S: O(n).
int longestChain(vector<int> nums, int k) {
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    int best = 1, run = 1;
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        if ((long long)nums[i] - nums[i - 1] < k) {
            run++;
        } else {
            run = 1;
        }
        best = max(best, run);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (longestChain(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-fit", {10, 1, 4, 7}, 4, 4);
    test("dups", {1, 1, 2}, 2, 2);
    test("k-one", {1, 2, 3}, 1, 1);
    test("break", {1, 5, 6, 20}, 5, 3);
    test("single", {5}, 3, 1);
}
