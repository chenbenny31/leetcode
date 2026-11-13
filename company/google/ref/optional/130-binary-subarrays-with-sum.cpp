// 130 · Binary Subarrays With Sum — LC 930
// nums holds only 0 / 1; return num of non-empty contiguous subarrays w/ sum = goal
//
// clarify:
// goal = 0 -> subarrays inside runs of zeros
// size of the cnt -> int is enough for n <= 3e4
// values really only 0 / 1? -> yes; window solution needs non-negative

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// solution 1: prefix cnt; also works w/ negatives (hash map instead of vector)
// core idea: subarray ending here sums to goal iff an earlier prefix = prefix - goal
// cnt prefixes seen so far by value (sums <= n, so a vector works)
// T: O(n), S: O(n)
int numSubarraysWithSum(const vector<int>& nums, int goal) {
    int n = nums.size();
    // count[p] = prefixes seen so far with sum p; count[0] is the empty prefix
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

// subarrays with sum <= k: shrink left while the window sum > k
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

// solution 2: sliding window; O(1) space, needs non-negative values
// core idea: exactly goal = atMost(goal) - atMost(goal - 1)
// T: O(n), S: O(1)
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

// follow-up:
// negatives allowed (LC 560) -> prefix hash map only; window needs non-negative
// nice subarrays w/ k odd nums (LC 1248) -> map value to value % 2, same code
