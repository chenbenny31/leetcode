// ByteDance (hack2hire) / LC 300: length of the longest strictly increasing
// subsequence of nums.
// Follow-up: the O(n^2) dp version.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Patience tails + hand-written lower bound. T: O(n log n), S: O(n).
int lengthOfLIS(const vector<int>& nums) {
    // tails[k] = smallest tail of any increasing subsequence of length k + 1
    vector<int> tails;
    for (int x : nums) {
        // lower bound: first tail >= x (>= keeps it strictly increasing)
        int lo = 0;
        int hi = tails.size();
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < x) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        if (lo == (int)tails.size()) {
            tails.push_back(x);
        } else {
            tails[lo] = x;
        }
    }
    return tails.size();
}

// Follow-up: dp over every earlier element. T: O(n^2), S: O(n).
int lengthOfLISQuadratic(const vector<int>& nums) {
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

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = lengthOfLIS(nums) == want && lengthOfLISQuadratic(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace-tail", {3, 1, 2, 4}, 3);
    test("all-equal", {7, 7, 7}, 1);
    test("decreasing", {3, 2, 1}, 1);
    test("empty", {}, 0);
}

// follow-up: O(n^2) dp -> see lengthOfLISQuadratic() above; dp[i] = 1 + max dp[j]
//   over j < i with nums[j] < nums[i]
// follow-up: print the LIS -> tailIdx[k] (index into nums) + parent[i] =
//   tailIdx[lo - 1]; walk parents back from tailIdx.back(), O(n log n)
// follow-up: non-decreasing -> upper bound (first tail > x) instead of >= x
// follow-up: envelopes (LC 354) -> sort w asc, h desc on ties, LIS on h, O(n log n)
