// 0/1 knapsack dp, T: O(n * sum), S: O(sum)

#include <vector>
#include <numeric> // std::accumulate
#include <cstdint> // uint8_t

class Solution {
public:
    bool canPartition(std::vector<int>& nums) {
        int total = std::accumulate(nums.begin(), nums.end(), 0);
        if (total % 2 != 0) { return false; }

        int target = total / 2;
        std::vector<uint8_t> dp(target + 1, 0);
        dp[0] = 1;

        for (int x : nums) {
            for (int i = target; i >= x; i--) {
                if (dp[i-x]) { dp[i] = 1; }
            }
        }
        return dp[target];
    }
};

// total%2 != 0: odd total early exit
// 0/1 knapsack: reverse inner loop d[i-x] uses preivous iteration's value, forward will count x multiple times
// bitset optimization: std::bitset<20001> dp with dp |= dp << x; shift and OR, O(n*sum/64) with SIMD
