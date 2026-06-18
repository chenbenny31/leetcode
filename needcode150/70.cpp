// dp, T: O(n), S: O(n)

#include <vector>

class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) { return n; }
        std::vector<int> dp(n + 1); // dp[i] = ways to arrive i-th floor
        dp[1] = 1;
        dp[2] = 2;
        for (int i = 3; i <= n; i++) { dp[i] = dp[i - 1] + dp[i - 2]; }
        return dp[n];
    }
};

// two variables, T: O(n), S: O(1)

class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) { return n; }
        int prev2 = 1, prev1 = 2;
        for (int i = 3; i <= n; i++) {
            int curr = prev1 + prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

// two-variable optimization: only prev1 and prev2 is needed
// follow-up matrix exponentiation: T: O(logn), climbStairs(n) = fib(n+1)
// generalize to k steps: dp[i] = sum(dp[i-1]..dp[i-k]), sliding-window sum in O(1) per step
