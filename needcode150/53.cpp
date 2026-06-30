// Kadane's Algorithm, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        int curr = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            curr = std::max(nums[i], curr + nums[i]);
            best = std::max(best, curr);
        }
        return best;
    }
};

// initailize from nums[0] not 0 or INT_MIN: all negative input, answer need to be the least negative one; INT_MIN risks overflow on curr + nums[i]
