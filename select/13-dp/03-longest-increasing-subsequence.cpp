// LC 300 Longest Increasing Subsequence (Medium) — O(n^2) DP / tails + lower bound
// Return the length of the longest strictly increasing subsequence of nums.
// Clarify: strictly increasing or non-decreasing; subsequence, not subarray; empty
//   input returns 0; length only, or the sequence itself.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: dp[i] = 1 + max dp[j] over j < i with nums[j] < nums[i]; the answer is
// the largest dp[i]. T: O(n^2), S: O(n).
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

// Core idea: keep the smallest tail for each length (tails stays sorted); each num
// replaces the first tail >= num (lower bound), or extends tails when there is none.
// T: O(n log n), S: O(n).
int lengthOfLISTails(const vector<int>& nums) {
    // tails[len] = smallest tail of any increasing subsequence of length len + 1
    vector<int> tails;
    for (int num : nums) {
        // lower bound: first tail >= num (>= keeps it strictly increasing)
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

// follow-up (ideas only):
// - print one LIS -> parent[i] = the j that set dp[i]; walk parents back from the
//   argmax of dp, then reverse
// - non-decreasing instead of strict -> replace the first tail > num (upper bound)
// - Russian doll envelopes (LC 354) -> sort by width ascending and height DESCENDING on
//   equal width (equal widths cannot chain), then LIS on the heights
// - longest string chain (LC 1048) -> sort words by length; dp[word] = 1 + best dp over
//   the words made by deleting one char, found by hash lookup instead of scanning all j
