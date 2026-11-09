// LC 209 Minimum Size Subarray Sum (Medium). Pattern: variable window, shrink while
// valid.
// nums all positive; return the minimal length of a contiguous subarray with sum
// >= target, or 0 if none.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// All values positive, so growing right raises the sum and shrinking left lowers it.
// T: O(n), S: O(1).
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
}

// follow-up: O(n log n) (LC asks) -> prefix[i] = sum of nums[0..i-1] is increasing;
//   for each i, j = lower_bound(prefix, prefix[i] + target); if j <= n: best = j - i
// follow-up: negatives allowed (LC 862) -> monotonic deque of prefix indices, O(n)
