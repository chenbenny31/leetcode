// 56 · Longest Increasing Subsequence — LC 300, 354, 1048
// Mother problem: select/13-dp/03-longest-increasing-subsequence.cpp
//
// Google reports: envelopes ×3, longest string chain
//
// LC 300: return the length of the longest strictly increasing subsequence of nums.
//
// Clarify
// - strictly increasing, gaps allowed; equal values never extend each other
// - return the length only
//
// Core idea
// - O(n^2) DP: dp[i] = 1 + max dp[j] over j < i with nums[j] < nums[i].
// - `lengthOfLISTails`: tails[len] = smallest tail of an increasing subsequence of
//   length len + 1; replace the first tail >= num (lower bound) or append.
// - T O(n^2) / O(n log n), S O(n).
//
// Tests
// - {3, 1, 2, 4} → 3 (tail replaced); zigzag → 4; duplicates {2, 2, 3, 3} → 2;
//   decreasing → 1
//
// Follow-ups
// - Russian doll envelopes (LC 354) → sort w asc and h desc on equal w, then tails LIS
//   on h
// - longest string chain (LC 1048) → sort by length, dp[word] = 1 + best dp over the
//   words made by deleting one char (hash map lookup)
// - print one LIS → parent[i] = the j that set dp[i], walk back from the argmax
// - non-decreasing → first tail > num instead of >= num

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// dp[i] = 1 + max dp[j] over j < i with nums[j] < nums[i]. T: O(n^2), S: O(n).
int lengthOfLIS(const vector<int>& nums) {
    int n = nums.size();
    // dp[i] = LIS length ending at nums[i]
    vector<int> dp(n, 1);
    int best = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        best = max(best, dp[i]);
    }
    return best;
}

// Keep the smallest tail per length, binary search where num goes.
// T: O(n log n), S: O(n).
int lengthOfLISTails(const vector<int>& nums) {
    // tails[len] = smallest tail of any increasing subsequence of length len + 1
    vector<int> tails;
    for (int num : nums) {
        // first tail >= num (>= keeps it strictly increasing)
        int lo = 0;
        int hi = tails.size();
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < num) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (lo == (int)tails.size()) {
            tails.push_back(num);
        } else {
            tails[lo] = num;
        }
    }
    return tails.size();
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = lengthOfLIS(nums) == want && lengthOfLISTails(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace-tail", {3, 1, 2, 4}, 3);
    test("zigzag", {1, 5, 2, 6, 3, 7}, 4);
    test("dups-strict", {2, 2, 3, 3}, 2);
    test("decreasing", {3, 2, 1}, 1);
}
