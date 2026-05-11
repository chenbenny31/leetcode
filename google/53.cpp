// Kadane's Algorithm, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        int curr = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            curr = std::max(nums[i], curr + nums[i]);
            best = std::max(best, curr);
        }
        return best;
    }
};

// curr = max(nums[i], curr + nums[i]): if curr + nums[i] < 0 then start fresh at nums[i]
// initailize from nums[0] over 0 or INT_MIN: answer is at least a negative element not 0, INT_MIN risk at overflow
// cache single sequentail pass: stride-1, prefetcher-friendly
