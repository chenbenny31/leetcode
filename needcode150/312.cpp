// interval dp, T: O(n3), S: O(n2)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxCoins(std::vector<int>(nums)) {
        int n = static_cast<int>(nums.size());

        std::vector<int> arr(n + 2); // pad with one sentinels on both ends
        arr[0] = arr[n+1] = 1;
        for (int i = 0; i < n; i++) { arr[i+1] = nums[i]; }

        std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, 0)); // dp[i][j] = max coins from bursting all balloons in (i, j)

        for (int len = 1; len <= n; len++) {
            for (int i = 1; i <= n - len + 1; i++) {
                int j = i + len - 1;
                for (int k = i; k <= j; k++) {
                    dp[i][j] = std::max(dp[i][j],
                                        dp[i][k-1] + arr[i-1] * arr[k] * arr[j+1] + dp[k+1][j]);
                }
            }
        }
        return dp[1][n];
    }
};

// reverse thinking - last ballon burst: fix k as last ballons burst in [i,j], neighbors arr[i-1] and arr[j+1] are boundaries
// sentinel padding: arr[0] = arr[n+1] = 1, boundary ballons have value 1 as outer neighbor
// interval dp: len from 1 to n, length-first iteration
