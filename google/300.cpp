// dp-1d, T: O(n2), S: O(n)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size()); // dp[i] = best len at nums[0..i]
        std::vector<int> dp(n, 1);
        int best = 1;

        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[j] < nums[i]) { dp[i] = std::max(dp[i], dp[j] + 1); }
            }
            best = std::max(best, dp[i]);
        }
        return best;
    }
};

// binary-search + patience-sorting, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::lower_bound

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        std::vector<int> tails;
        tails.reserve(nums.size()); // tails[i] = min tail of IS of len i+1

        for (int x : nums) {
            auto it = std::lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) { tails.push_back(x); }
            else { *it = x; }
        }
        return static_cast<int>(tails.size());
    }
};

// tails invariant: tails is strictly incre, lower_bound is valid, tails[i]
// *it = x not break LIS len: len unchanged from replacement, only alter the vals
// all equal vals: lower_bound always find index 0 and replace tails[0], size stays at 1
