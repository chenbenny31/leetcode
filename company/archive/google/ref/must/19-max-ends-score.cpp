// score of nums[i..j] (i < j) = nums[i] + nums[j] - sum of elements strictly between
// return max score over all such subarrays
//
// clarify:
// n >= 2 -> at least one pair exists
// adjacent i, j -> empty middle, score = nums[i] + nums[j]
// negative values -> allowed; best starts at INT_MIN
// value range -> int here; long long is a follow-up
// return score or indices? -> score; indices are a follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// solution 1: brute force; baseline to check solution 2
// core idea: fix first idx, extend last idx, keep the middle sum
// T: O(n^2), S: O(1)
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

// solution 2: Kadane-style one pass; the expected answer
// core idea: left = best (first - middle) over subarrays ending right before j
// nums[j] starts a new one or joins the middle (left - nums[j]); only the best is kept
// T: O(n), S: O(1)
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

// follow-up:
// return indices (i, j) -> leftStart = j when nums[j] restarts left; record on improve
// j - i >= d -> score = (prefix[i + 1] + nums[i]) + (nums[j] - prefix[j]); max 1st term
// values up to 1e9 -> long long for best / left / middle
