// dp

class Solution {
private:
    int robRange(std::vector<int>& nums, int first, int last) {
        int prev1 = 0;
        int prev2 = 0;
        for (int i = first; i <= last; i++) {
            int curr = std::max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }

public:
    int rob(vector<int>& nums) {
        const int n = static_cast<int>(nums.size());    
        if (n == 1) { return nums[0]; }
        return std::max(robRange(nums, 0, n - 2),
                        robRange(nums, 1, n - 1));
    }
};
