// LC 239 Sliding Window Maximum (Hard) — monotonic decreasing deque
// window of size k slides over nums one step at a time; return max of each window
//
// clarify:
// k range -> 1 <= k <= n
// empty nums -> empty result
// dups -> ok, an equal older value is popped too
// negative values -> ok

#include <iostream>
#include <vector>
#include <string>
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::deque;

// core idea: deque of window idx w/ decreasing values; front = max
// new value pops every smaller-or-equal one behind it
// T: O(n), S: O(k) — each idx pushed and popped once
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    // dq = idx in window, values strictly decreasing front to back
    deque<int> dq;
    vector<int> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // front idx has left the window
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
    return 0;
}

// follow-up:
// simpler first answer -> max-heap of (value, idx), pop top while stale; O(n log n)
// longest subarray w/ max - min <= limit (LC 1438) -> max deque + min deque, shrink left
