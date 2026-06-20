// dp, T: O(mn), n = target, m = nums size, S: O(n)

#include <vector>
#include <climits> // INT_MAX

class Solution {
public:
    int combinationSum4(std::vector<int>& nums, int target) {
        std::vector<unsigned int> dp(target + 1, 0);
        dp[0] = 1; // empty combination

        for (int i = 1; i <= target; i++) { // outer: target, count once
            for (int x : nums) { // inner: try all items, allow duplicate
                if (x <= i && dp[i-x] <= INT_MAX - dp[i]) {
                    dp[i] += dp[i-x];
                }
            }
        }
        return static_cast<int>(dp[target]);
    }
};

// order matters: outer loop on target not nums, for permutations, all orderings count
// unsigned int not int: 32-bit signed guaranteed
// dp[i-x] <= INT_MAX - dp[i]: overflow guard
