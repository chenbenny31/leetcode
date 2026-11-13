// 98 · Sliding Window Maximum — LC 239
// Mother: select/03-sliding-window/04-sliding-window-max.cpp
// window of size k slides over nums one step at a time; return max of each window
//
// clarify:
// 1 <= k <= n -> output has n - k + 1 values
// dups -> pop back on <=, keep the newer idx
// negatives -> fine, no sentinel value used

#include <iostream>
#include <vector>
#include <string>
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::deque;

// core idea: deque of idx w/ values strictly decreasing front -> back; front = window max
// pop front when it leaves the window; pop back while <= nums[i]
// T: O(n), S: O(k)
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    // idx in window, values strictly decreasing front -> back
    deque<int> dq;
    vector<int> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // smaller older value can never be max while nums[i] is in window
        while (!dq.empty() && nums[dq.back()] <= nums[i]) {
            dq.pop_back();
        }
        dq.push_back(i);
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    cout << label << (maxSlidingWindow(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("max-expires", {4, 2, 12, 3}, 2, {4, 12, 12});
    test("decreasing", {5, 3, 1}, 2, {5, 3});
    test("dups", {2, 2, 1}, 2, {2, 2});
    test("k-equals-n", {1, 3, 2}, 3, {3});
}

// follow-up:
// simpler first answer -> max-heap of (value, idx), lazily pop expired tops; O(n log n)
// max - min <= limit (LC 1438) -> max deque + min deque, shrink left while fronts > limit
