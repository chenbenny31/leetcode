// dp, T: O(n), S: O(n)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int rob(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        if (n == 1) { return nums[0]; }
        std::vector<int> dp(n);
        dp[0] = nums[0];
        dp[1] = std::max(nums[0], nums[1]);
        for (int i = 2; i < n; i++) {
            dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i]);
        }
        return dp[n - 1];
    }
};

// two variables, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int rob(std::vector<int>& nums) {
        int n =static_cast<int>(nums.size());
        if (n == 1) { return nums[0]; }
        int prev2 = nums[0];
        int prev1 = std::max(nums[0], nums[1]);
        for (int i = 2; i < n; i++) {
            int curr = std::max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

// dp[i] = max(dp[i-1], dp[i-2] + nums[i]), two choices per house, greedy doesn't work as adjacent forbidden, must consider global optimum
