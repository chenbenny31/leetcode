// sign-marking

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findErrorNums(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        int dup = -1;

        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]) - 1;
            if (nums[idx] < 0) { dup = std::abs(nums[i]); }
            else { nums[idx] *= -1; }
        }

        int miss = -1;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) { miss = i + 1; }
        }
        return {dup, miss};
    }
};
