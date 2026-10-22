// LC 673 Number of Longest Increasing Subsequences: return how many strictly
// increasing subsequences of nums have the maximum length.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// DP over end index: for each i, extend every smaller earlier j and merge counts of
// equal lengths. T: O(n^2), S: O(n).
int findNumberOfLIS(const vector<int>& nums) {
    int n = nums.size();
    vector<int> len(n, 1);   // len[i] = longest increasing subsequence ending at i
    vector<int> cnt(n, 1);   // cnt[i] = how many of that length end at i
    int best = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] >= nums[i]) { continue; }
            if (len[j] + 1 > len[i]) {
                len[i] = len[j] + 1;
                cnt[i] = cnt[j];
            } else if (len[j] + 1 == len[i]) {
                cnt[i] += cnt[j];
            }
        }
        best = max(best, len[i]);
    }
    int total = 0;
    for (int i = 0; i < n; i++) {
        if (len[i] == best) { total += cnt[i]; }
    }
    return total;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findNumberOfLIS(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-ways", {1, 3, 5, 4, 7}, 2);
    test("all-equal", {2, 2, 2}, 3);
    test("decreasing", {5, 4, 3}, 3);
    test("empty", {}, 0);
}
