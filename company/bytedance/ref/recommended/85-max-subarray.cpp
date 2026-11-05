// ByteDance (hack2hire) / LC 53: return the largest sum of a non-empty contiguous
// subarray of nums.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::pair;

// Kadane: best sum ending at i extends the one ending at i-1 or restarts.
// T: O(n), S: O(1).
int maxSubArray(const vector<int>& nums) {
    // best sum of a subarray ending at i
    int cur = nums[0];
    // best sum seen anywhere
    int best = nums[0];
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        // restart when the prefix hurts
        cur = max(nums[i], cur + nums[i]);
        best = max(best, cur);
    }
    return best;
}

// follow-up: Kadane, remembering where the current run started. T: O(n), S: O(1).
pair<int, int> maxSubArrayBounds(const vector<int>& nums) {
    int cur = nums[0];
    int best = nums[0];
    // start index of the run ending at i
    int start = 0;
    // bounds of the best run
    int bestL = 0;
    int bestR = 0;
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        if (cur < 0) {
            cur = nums[i];
            start = i;
        } else {
            cur += nums[i];
        }
        if (cur > best) {
            best = cur;
            bestL = start;
            bestR = i;
        }
    }
    return {bestL, bestR};
}

void test(const string& label, const vector<int>& nums, int want, int wantL, int wantR) {
    pair<int, int> bounds = maxSubArrayBounds(nums);
    bool ok = maxSubArray(nums) == want && bounds.first == wantL
              && bounds.second == wantR;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6, 3, 6);
    test("single", {1}, 1, 0, 0);
    test("all-negative", {-3, -1, -2}, -1, 1, 1);
}

// follow-up: return bounds -> see maxSubArrayBounds() above; restart at i when cur < 0
// follow-up: circular (LC 918) -> max(maxKadane, total - minKadane); if all negative
//   return maxKadane (else it picks the empty subarray)
// follow-up: max product (LC 152) -> track curMax and curMin; swap them when x < 0
// follow-up: range queries -> segment tree node (sum, bestPrefix, bestSuffix, best),
//   O(log n) per query
