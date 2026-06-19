// two pass, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int rob(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        if (n == 1) { return nums[0]; }
        if (n == 2) { return std::max(nums[0], nums[1]); }
        return std::max(rob_range(nums, 0, n - 2),
                        rob_range(nums, 1, n - 1));
    }

private:
    int rob_range(std::vector<int>& nums, int lo, int hi) {
        int prev2 = 0, prev1 = 0;
        for (int i = lo; i <= hi; i++) {
            int curr = std::max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

// why two passes suffice: either house 0 is robbed or not, mutually exclusive: rob 0 means can't rob n-1 -> [0, n-2]; otherwise [1, n-1]
