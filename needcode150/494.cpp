// dfs + memoization, T: O(n * sum), S: O(n * sum)

#include <vector>
#include <unordered_map>
#include <climits> // INT_MIN

class Solution {
public:
    int findTargetSumWays(std::vector<int>& nums, int target) {
        int n = static_cast<int>(nums.size());
        int total = 0;
        for (int num : nums) { total += num; }

        // sum range [-total, total], offset by total
        std::vector<std::vector<int>> memo(n, std::vector<int>(2 * total + 1, INT_MIN));
        return dfs(nums, memo, 0, 0, target, total);
    }

private:
    int dfs(std::vector<int>& nums,
            std::vector<std::vector<int>>& memo,
            int idx, int sum, int target, int total) {
        if (idx == static_cast<int>(nums.size())) { return sum == target ? 1 : 0; }

        if (memo[idx][sum+total] != INT_MIN) { return memo[idx][sum+total]; }

        int result = dfs(nums, memo, idx + 1, sum + nums[idx], target, total) +
                     dfs(nums, memo, idx + 1, sum - nums[idx], target, total);
        memo[idx][sum+total] = result;
        return result;
    }
};

// 0/1 knapsack dp, T: O(n * sum), S: O(sum)

#include <vector>
#include <numeric> // std::accumulate
#include <cstdlib> // std::abs

class Solution {
public:
    int findTargetSumWays(std::vector<int>& nums, int target) {
        int total = std::accumulate(nums.begin(), nums.end(), 0);
        if (std::abs(target) > total) { return 0; } // unreachable
        if ((total + target) % 2 != 0) { return 0; } // p not integer

        int goal = (total + target) / 2; // num of ways to assign '+' signs, postive component
        std::vector<int> dp(goal + 1, 0);
        dp[0] = 1;

        for (int num : nums) {
            for (int j = goal; j >= num; j--) {
                dp[j] += dp[j-num];
            }
        }
        return dp[goal];
    }
};

// P = (total + target) / 2: P - N = target && P + N = total
// parity + range guards: O(1) pruning 
// 0/1 knapsack: reverse inner loop, each num used exactly once
