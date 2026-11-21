// 03 · Longest Subarray With At Most K Distinct (Cisco OA, Medium) — LC 340 on ints
// Mother: select/03-sliding-window/01-longest-substring-no-repeat.cpp
// return len of the longest contiguous subarray w/ at most k distinct values
//
// clarify:
// k == 0 -> 0
// empty array -> 0
// value range -> any int, so count in a hash map
// return len or the subarray -> len

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

// core idea: grow right; while the window has > k distinct values, drop from the left
// T: O(n), S: O(k) — each idx enters and leaves the window once
int longestAtMostK(const vector<int>& nums, int k) {
    int n = nums.size();
    // value -> cnt inside the window
    unordered_map<int, int> cnt;
    int best = 0;
    int left = 0;
    for (int right = 0; right < n; right++) {
        cnt[nums[right]]++;
        while ((int)cnt.size() > k) {
            int out = nums[left];
            cnt[out]--;
            // erase at 0 so cnt.size() stays the num of distinct values
            if (cnt[out] == 0) { cnt.erase(out); }
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (longestAtMostK(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {1, 2, 1, 2, 3}, 2, 4);
    test("one", {4, 4, 5, 5, 5}, 1, 3);
    test("k-zero", {1, 2}, 0, 0);
    test("k-large", {1, 2, 3}, 5, 3);
    return 0;
}

// follow-up:
// k == 2 (LC 904 fruit into baskets) -> same code
// num of subarrays w/ exactly k distinct (LC 992) -> atMost(k) - atMost(k - 1)
// shortest subarray w/ >= k distinct -> company/google/ref/must/20
// values in 0..m -> vector<int> cnt(m + 1) + a `distinct` counter, no hash map
// return the subarray -> remember bestLeft when best improves
