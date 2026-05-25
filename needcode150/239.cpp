// monotonic-deque, T: O(n), S: O(k)

#include <vector>
#include <deque>

class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        std::vector<int> out;
        out.reserve(n - k + 1);
        std::deque<int> dq; // indices, front = max of curr win

        for (int right = 0; right < n; right++) {
            // evict indices outside win
            if (!dq.empty() && dq.front() < right - k + 1) { dq.pop_front(); }

            // maintain decre order, pop smaller elem from back
            while (!dq.empty() && nums[dq.back()] < nums[right]) { dq.pop_back(); }

            dq.push_back(right);
            if (right >= k - 1) { out.push_back(nums[dq.front()]); }
        }
        return out;
    }
};

// store indices not vals: need to evict elem by index; dq.front() < right - k + 1 for win verify
// std::deque overhead: seg buf cause ptr chasing, replace with flat circular buf for bounded k
// amortized O(1) per elem: each idx push and pop once
// extend to sliding win min: maintain incre deque, pop from back when nums[dq.back()] > nums[right])
