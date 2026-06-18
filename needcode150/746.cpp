// dp, T: O(n), S: O(n)

#include <vector>
#include <algorithm> // std::min

class Solution {
public:
    int minCostClimbingStairs(std::vector<int>& cost) {
        int n = static_cast<int>(cost.size());
        std::vector<int> dp(n + 1, 0);

        for (int i = 2; i <= n; i++) {
            dp[i] = std::min(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
        }
        return dp[n];
    }
};

// two variables, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::min

class Solution {
public:
    int minCostClimbingStairs(std::vector<int>& cost) {
        int n = static_cast<int>(cost.size());
        int prev2 = 0, prev1 = 0;

        for (int i = 2; i <= n; i++) {
            int curr = std::min(prev1 + cost[i - 1], prev2 + cost[i - 2]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
