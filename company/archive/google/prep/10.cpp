// neg vals, k <= 0 -> no sliding window
// overlapping subarrays
// sum within int

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::max;

// prefix[j] - prefix[i] = k means [i..j-1] sum to k, need count to accum
// T: O(n), S: O(n)
int subarraySum(const vector<int>& nums, int k) {
    unordered_map<int, int> freq; // prefix-sum: its count 
    freq[0] = 1;
    int prefix = 0;
    int count = 0;

    for (int num : nums) {
        prefix += num;
        auto it = freq.find(prefix - k);
        if (it != freq.end()) {
            count += it->second;
        }

        freq[prefix]++;
    }
    return count;
}

// don't pick x, find it by slice left/right into sum of vals L, R
// x in right means L == R - x, x = total - 2L, else L - x = R, x = total - 2R
bool canSplitAfterRemoval(const vector<int>& nums) {
    int n = nums.size();
    if (n < 3) { return false; }
    vector<int> prefix(n + 1, 0); // prefix[i] = sum of [0..i)
    for (int i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + nums[i];
    }
    int total = prefix[n];

    unordered_set<int> rightvals{nums[n - 1]};
    for (int i = n - 2; i >= 1; i--) {
        rightvals.insert(nums[i]);
        if (rightvals.count(total - 2 * prefix[i])) { return true; }
    }

    unordered_set<int> leftvals{nums[0]};
    for (int i = 2; i <= n - 1; i++) {
        leftvals.insert(nums[i - 1]);
        int right = total - prefix[i];
        if (leftvals.count(total - 2 * right)) { return true; }
    }
    return false;
}

// 2d diff array, then 2d prefix sum, T: O(rects + mn), S: O(mn)
int maxRectangleOverlap(const vector<vector<int>>& rects, int m, int n) {
    vector<vector<int>> diff(m + 1, vector<int>(n + 1, 0));
    for (const auto& r : rects) {
        int r1 = r[0];
        int c1 = r[1];
        int r2 = r[2];
        int c2 = r[3];

        diff[r1][c1]++;
        diff[r2 + 1][c2 + 1]++;
        diff[r1][c2 + 1]--;
        diff[r2 + 1][c1]--;
    }

    int maxArea = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (r > 0) { diff[r][c] += diff[r - 1][c]; }
            if (c > 0) { diff[r][c] += diff[r][c - 1]; }
            if (r > 0 && c > 0) { diff[r][c] -= diff[r - 1][c - 1]; }
            maxArea = max(maxArea, diff[r][c]);
        }
    }
    return maxArea;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

void testSplit(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canSplitAfterRemoval(nums) == want ? " PASS\n" : " FAIL\n");
}

void testOverlap(const string& label, const vector<vector<int>>& rects, int m, int n,
                 int want) {
    cout << label << (maxRectangleOverlap(rects, m, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {1, 2, 1}, 3, 2);
    test("negative-zero-k", {1, -1, 0}, 0, 3);
    test("negative-k", {-1, 1, -1, -1}, -1, 5);
    test("none", {1, 2}, 5, 0);
    testSplit("split-remove-first", {7, 1, 1, 2}, true);
    testSplit("split-remove-last", {2, 1, 1, 7}, true);
    testSplit("split-none", {1, 2, 4}, false);
    testSplit("split-too-short", {1, 1}, false);
    testOverlap("overlap-corner", {{0, 0, 1, 1}, {1, 1, 2, 2}}, 3, 3, 2);
    testOverlap("overlap-nested", {{0, 0, 2, 2}, {0, 0, 1, 1}, {1, 1, 1, 1}}, 3, 3, 3);
    testOverlap("overlap-none", {}, 2, 2, 0);
}

