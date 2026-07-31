// Modified Kadene's

#include <vector>
#include <algorithm> // std::max, std::min, std::swap

class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        int curMax = nums[0];
        int curMin = nums[0];
        int res = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] < 0) { std::swap(curMax, curMin); }
            curMax = std::max(nums[i], curMax * nums[i]);
            curMin = std::min(nums[i], curMin * nums[i]);
            res = std::max(res, curMax);
        }
        return res;
    }
};
