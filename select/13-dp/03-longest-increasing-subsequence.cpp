// LC 300 Longest Increasing Subsequence (Medium). Pattern: 1D DP; patience tails.
// Return the length of the longest strictly increasing subsequence of nums.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;
using std::sort;

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

// Follow-up: keep the smallest tail per length, binary search where num goes.
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

// LC 354: envelopes [w, h] nest iff both strictly smaller; max nesting depth. Sort w asc
// and h desc on equal w (equal widths can't chain then), then LIS on h.
// T: O(n log n), S: O(n).
int maxEnvelopes(vector<vector<int>> envelopes) {
    sort(envelopes.begin(), envelopes.end(),
         [](const vector<int>& a, const vector<int>& b) {
             if (a[0] != b[0]) { return a[0] < b[0]; }
             return a[1] > b[1];
         });
    // heights in sorted order
    vector<int> heights;
    for (const auto& env : envelopes) { heights.push_back(env[1]); }
    return lengthOfLISTails(heights);
}

// LC 1048: a is a predecessor of b if inserting one char into a gives b; longest chain.
// Sort by length, then dp[word] = 1 + best dp over the words made by deleting one char.
// T: O(n log n + n L^2), S: O(n L); L = max word length.
int longestStrChain(vector<string> words) {
    sort(words.begin(), words.end(),
         [](const string& a, const string& b) { return a.size() < b.size(); });
    // dp[word] = longest chain ending at word
    unordered_map<string, int> dp;
    int best = 0;
    for (const string& word : words) {
        int chain = 1;
        for (int i = 0; i < (int)word.size(); i++) {
            string prev = word.substr(0, i) + word.substr(i + 1);
            if (dp.count(prev)) { chain = max(chain, dp[prev] + 1); }
        }
        dp[word] = chain;
        best = max(best, chain);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = lengthOfLIS(nums) == want && lengthOfLISTails(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testEnvelopes(const string& label, const vector<vector<int>>& envelopes, int want) {
    cout << label << (maxEnvelopes(envelopes) == want ? " PASS\n" : " FAIL\n");
}

void testStrChain(const string& label, const vector<string>& words, int want) {
    cout << label << (longestStrChain(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace-tail", {3, 1, 2, 4}, 3);
    test("zigzag", {1, 5, 2, 6, 3, 7}, 4);
    test("dups-strict", {2, 2, 3, 3}, 2);
    test("decreasing", {3, 2, 1}, 1);
    testEnvelopes("env-unsorted", {{3, 5}, {1, 2}, {3, 3}, {2, 4}}, 3);
    testEnvelopes("env-same-width", {{2, 3}, {2, 4}}, 1);
    testEnvelopes("env-same-height", {{1, 2}, {2, 2}}, 1);
    testEnvelopes("env-nested", {{1, 1}, {2, 2}, {3, 3}}, 3);
    testStrChain("chain-unsorted", {"abc", "a", "ab"}, 3);
    testStrChain("chain-delete-middle", {"abc", "ac", "c"}, 3);
    testStrChain("chain-skip-length", {"a", "abc"}, 1);
    testStrChain("chain-two-roots", {"a", "b", "ab"}, 2);
}

// follow-up: print one LIS -> parent[i] = the j that set dp[i]; walk parents back from
//   the argmax of dp, then reverse
// follow-up: non-decreasing -> first tail > num instead of >= num
// follow-up: LC 1048 vs LIS -> same DP over a sorted order (by length here); the
//   predecessor is found by deleting one char + hash lookup instead of scanning all j < i
