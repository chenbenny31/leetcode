// LC 930 Binary Subarrays With Sum: nums holds only 0 / 1, 0 <= goal <= n. Return the
// number of non-empty contiguous subarrays whose sum equals goal.
//
// Clarify:
// - goal = 0 -> counts the subarrays inside runs of zeros
// - count up to n^2 / 2 -> fits in int for n <= 3e4
// Idea: prefix sums. A subarray ending here has sum goal iff an earlier prefix equals
//   prefix - goal, so count earlier prefixes by value (sums <= n, a vector works).
//   O(1) space: atMost(goal) - atMost(goal - 1) with a sliding window.
// Pitfalls:
// - count[0] = 1 for the empty prefix, else subarrays starting at 0 are missed
// - add count[prefix - goal] before count[prefix]++ (goal 0 would count itself)
// - atMost(-1) = 0; the window only works because values are non-negative
// Tests to write: mixed 1s and 0s; all zeros with goal 0; goal above the total;
//   single element.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// prefix sum + count of earlier prefixes. T: O(n), S: O(n).
int numSubarraysWithSum(const vector<int>& nums, int goal) {
    int n = nums.size();
    // count[p] = prefixes seen so far with sum p
    vector<int> count(n + 1, 0);
    count[0] = 1;
    int prefix = 0;
    int ans = 0;
    for (int x : nums) {
        prefix += x;
        if (prefix - goal >= 0) { ans += count[prefix - goal]; }
        count[prefix]++;
    }
    return ans;
}

// subarrays with sum <= k: shrink left while the window sum > k. T: O(n), S: O(1).
int atMost(const vector<int>& nums, int k) {
    if (k < 0) { return 0; }
    int left = 0;
    int sum = 0;
    int ans = 0;
    for (int right = 0; right < (int)nums.size(); right++) {
        sum += nums[right];
        while (sum > k) {
            sum -= nums[left];
            left++;
        }
        ans += right - left + 1;
    }
    return ans;
}

// exactly goal = atMost(goal) - atMost(goal - 1). T: O(n), S: O(1).
int numSubarraysWindow(const vector<int>& nums, int goal) {
    return atMost(nums, goal) - atMost(nums, goal - 1);
}

void test(const string& label, const vector<int>& nums, int goal, int want) {
    bool ok = numSubarraysWithSum(nums, goal) == want &&
              numSubarraysWindow(nums, goal) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {1, 0, 1, 0, 1}, 2, 4);
    test("all-zeros", {0, 0, 0}, 0, 6);
    test("goal-too-big", {1, 0, 1}, 3, 0);
    test("single", {1}, 1, 1);
}

// follow-up: negatives allowed (LC 560) -> prefix hash map only; the window needs
//   non-negative values
// follow-up: nice subarrays with k odds (LC 1248) -> map each value to value % 2, same
