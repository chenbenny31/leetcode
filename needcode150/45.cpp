// dp bottom up, T: O(n2), S: O(n)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int jump(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> dp(n, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] != INT_MAX && j + nums[j] >= i) {
                    dp[i] = std::min(dp[i], dp[j] + 1);
                }
            }
        }
        return dp[n - 1];
    }
};

// greedy, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int jump(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int jumps = 0;
        int curEnd = 0;
        int farthest = 0;

        for (int i = 0; i < n - 1; i++) {
            farthest = std::max(farthest, i + nums[i]);
            if (i == curEnd) {
                jumps++;
                curEnd = farthest;
            }
        }
        return jumps;
    }
};

// greedy correctness: maintain farthest as state, update with all possible candidate by i + nums[i], trigger jumps++
