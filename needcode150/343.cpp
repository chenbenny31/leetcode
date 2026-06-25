// dp, T: O(n2), S: O(n)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int integerBreak(int n) {
        std::vector<int> dp(n + 1, 0);
        dp[1] = 1;

        for (int i = 2; i <= n; i++) {
            for (int j = 1; j < i; j++) {
                dp[i] = std::max(dp[i], std::max(j * (i-j),
                                                 j * dp[i-j]));
            }
        }
        return dp[n];
    }
};

// max(j*(i-j), j*dp[i-1]): keep i-j (j*(i-j)) or break it further j*dp[i-j]
