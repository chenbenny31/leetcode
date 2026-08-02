// max-heap + lazy-del

#include <vector>
#include <queue>
#include <utility>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::priority_queue<std::pair<int, int>> maxHeap; // store (val, idx)
        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            maxHeap.push({nums[i], i});
            while (maxHeap.top().second <= i - k) { maxHeap.pop(); } // ensure the top cand is in win
            if (i >= k - 1) { out.push_back(maxHeap.top().first); }
        }
        return out;
    }
};

// mono-deque

#include <deque>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::deque<int> win; // store idx with decre vals
        std::vector<int> out;
        out.reserve(n - k + 1);

        for (int i = 0; i < n; i++) {
            while (!win.empty() && nums[win.back()] <= nums[i]) {
                win.pop_back(); 
            }
            win.push_back(i);

            if (win.front() <= i - k) { win.pop_front(); }

            if (i >= k - 1) {
                out.push_back(nums[win.front()]);
            }
        }
        return out;
    }
};
