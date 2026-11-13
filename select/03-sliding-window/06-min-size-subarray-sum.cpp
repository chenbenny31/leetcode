// LC 209 Minimum Size Subarray Sum (Medium) — variable window, shrink while valid
// nums are all positive; return the minimal length of a contiguous subarray with
// sum >= target, or 0 if there is none.
// Clarify: all values positive (no zero / negative); sum fits in int; no valid subarray
//   returns 0; return the length, not the subarray.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Core idea: values are positive, so moving right raises the sum and moving left lowers
// it; for each right, shrink while the window still reaches target. T: O(n), S: O(1).
int minSubArrayLen(int target, const vector<int>& nums) {
    int n = nums.size();
    // n + 1 = no valid window yet
    int best = n + 1;
    // sum of nums[left..right]
    int sum = 0;
    int left = 0;
    for (int right = 0; right < n; right++) {
        sum += nums[right];
        while (sum >= target) {
            best = min(best, right - left + 1);
            sum -= nums[left];
            left++;
        }
    }
    return best == n + 1 ? 0 : best;
}

void test(const string& label, int target, const vector<int>& nums, int want) {
    cout << label << (minSubArrayLen(target, nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", 5, {1, 4, 1}, 2);
    test("one-element", 4, {1, 1, 4}, 1);
    test("whole-array", 6, {1, 2, 3}, 3);
    test("impossible", 10, {1, 2, 3}, 0);
    return 0;
}

// follow-up (ideas only):
// - O(n log n) version (LC asks for it) -> prefix sums are increasing; for each start i
//   binary search the first end j with prefix[j] >= prefix[i] + target
// - negatives allowed (LC 862) -> the window no longer works; monotonic deque of prefix
//   indices with increasing prefix values, O(n)
