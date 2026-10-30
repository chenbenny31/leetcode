// The score of a subarray nums[i..j] (i < j) is nums[i] + nums[j] minus the sum of the
// elements strictly between them. Return the maximum score over all such subarrays.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// straightforward: fix the first index, extend the last one, keep the middle sum.
// T: O(n^2), S: O(1).
int maxEndsScore(const vector<int>& nums) {
    int n = nums.size();
    int best = INT_MIN;
    for (int i = 0; i < n; i++) {
        // sum of nums[i+1..j-1]
        int middle = 0;
        for (int j = i + 1; j < n; j++) {
            best = max(best, nums[i] + nums[j] - middle);
            middle += nums[j];
        }
    }
    return best;
}

// improved: left = best (first - middle) over subarrays that end right before j.
// nums[j] either starts a new one, or extends the old one and becomes a middle.
// Two starts get the same future updates, so only the best one matters.
// T: O(n), S: O(1).
int maxEndsScoreFast(const vector<int>& nums) {
    int n = nums.size();
    int best = INT_MIN;
    int left = nums[0];
    for (int j = 1; j < n; j++) {
        best = max(best, left + nums[j]);
        left = max(nums[j], left - nums[j]);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = maxEndsScore(nums) == want && maxEndsScoreFast(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", {4, 5}, 9);
    test("restart", {1, 9, 1, 9}, 17);
    test("neg-mid", {1, -5, 1}, 7);
    test("all-neg", {-1, -2, -3}, -2);
}
