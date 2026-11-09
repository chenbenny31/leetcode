// LC 239 Sliding Window Maximum (Hard). Pattern: monotonic decreasing deque.
// A window of size k slides over nums one step at a time; return the max of each
// window.

#include <iostream>
#include <vector>
#include <string>
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::deque;

// Deque holds indices whose values decrease front to back; front is the window max.
// T: O(n), S: O(k).
vector<int> maxSlidingWindow(const vector<int>& nums, int k) {
    // indices in the window, values strictly decreasing front to back
    deque<int> dq;
    vector<int> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // front index has left the window
        if (!dq.empty() && dq.front() <= i - k) {
            dq.pop_front();
        }
        // a smaller older value can never be a max while nums[i] is in the window
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

// follow-up: simpler first answer -> max-heap of (value, index), lazily pop expired
//   tops, O(n log n)
// follow-up: max - min <= limit (LC 1438) -> maxDq + minDq; shrink left while the
//   fronts differ by > limit, O(n)
