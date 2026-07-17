// max-heap with lazy-deletion, T: O(nlogn), S: O(n)

#include <vector>
#include <queue>
#include <utility> // std::pair

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::priority_queue<std::pair<int, int>> maxHeap;
        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            maxHeap.push({nums[i], i});
            while (maxHeap.top().second <= i - k) { maxHeap.pop(); }
            if (i >= k - 1) { out.push_back(maxHeap.top().first); }
        }
        return out;
    }
};

// monotonic-deque, T: O(n), S: O(k)

#include <vector>
#include <deque>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<Int>(nums.size());
        std::deque<int> window; // indices, values strictly decreasing front to back

        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            while (!window.empty() && nums[window.back()] <= nums[i]) { window.pop_back(); }
            window.push_back(i);
            if (window.front() <= i - k) { window.pop_front(); }
            if (i >= k - 1) { out.push_back(nums[window.front()]); }
        }
        return out;    
    }
};

// deque: holds candidates in flight with strictly decreasing order
// heap T: O(nlogn): lazy deletion
