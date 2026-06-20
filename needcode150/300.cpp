// dp, T: O(n2), S: O(n)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> dp(n, 1);
        int best = 1;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {
                    dp[i] = std::max(dp[i], dp[j] + 1);
                }
                best = std::max(best, dp[i]);
            }
        }
        return best;
    }
};

// binary-search, patience-sorting, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::lower_bound

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        std::vector<int> tails;
        tails.reserve(nums.size());

        for (int x : nums) {
            auto it = std::lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) { tails.push_back(x); }
            else { *it = x; }
        }
        return static_cast<int>(tails.size());
    }
};

// tails invariant: always strictly increasing, lower_bound find first element >= x, replace *it = x keeps tails as small as possible
// tails it not the LIS: tails only give correct length
