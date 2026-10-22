// max-heap with lazy-del, T: O(nlogn), S: O(n)

#include <vector>
#include <queue> // std::priority_queue
#include <utility> // std::pair

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::priority_queue<std::pair<int, int>> maxHeap; // (val, idx), cmp by val first

        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            maxHeap.push({nums[i], i});
            // only the top has to be in-window, stale pairs underline are harmless
            while (maxHeap.top().second <= i - k) { maxHeap.pop(); }
            if (i >= k - 1) { out.push_back(maxHeap.top().first); }
        }
        return out;
    }
};

// mono-deque, T: O(n), S: O(k)

#include <vector>
#include <deque>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::deque<int> win; // indices of vals non-incre front to back, front is win max

        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            while (!win.empty() && nums[win.back()] <= nums[i]) { win.pop_back(); }
            win.push_back(i);

            if (win.front() <= i - k) { win.pop_front(); }
            if (i >= k - 1) { out.push_back(nums[win.front()]); }
        }
        return out;
    }
};
