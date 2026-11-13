// LC 300 Longest Increasing Subsequence (Medium) — O(n^2) DP / tails + lower bound
// return len of the longest strictly increasing subsequence of nums
//
// clarify:
// strict or non-decreasing? -> strict; non-decreasing -> upper bound instead
// subsequence, not subarray -> gaps allowed
// empty input -> 0
// len only? -> yes; the sequence itself -> follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// solution 1: O(n^2) DP; easy to rebuild the sequence or change the condition
// core idea: dp[i] = 1 + max dp[j] over j < i w/ nums[j] < nums[i]; answer = max dp[i]
// T: O(n^2), S: O(n)
int lengthOfLIS(const vector<int>& nums) {
    int n = nums.size();
    // dp[i] = LIS len ending at nums[i]
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

// solution 2: tails + binary search; use it when n is large
// core idea: keep min tail per len (sorted); num replaces first tail >= num, else extends
// T: O(n log n), S: O(n)
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

// follow-up:
// print one LIS -> parent[i] = j that set dp[i]; walk back from argmax, reverse
// non-decreasing -> replace first tail > num (upper bound)
// Russian doll envelopes (LC 354) -> sort width asc, height DESC on ties; LIS on heights
// longest string chain (LC 1048) -> sort by len; dp[word] = 1 + best dp w/ 1 char deleted
