// dp

#include <vector>
#include <algorithm>

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        std::vector<int> dp(n, 1); // dp[i] = max len of LIS until i
        int res = 1;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) { dp[i] = std::max(dp[i], dp[j] + 1); }
            }
            res = std::max(res, dp[i]);
        }
        return res;
    }
};

// patience-sort, binary-search

class Solution {
public:
    int lengthOfLIS(std::vector<int>& nums) {
        std::vector<int> tails; // tails[k] = smalles tail val of len k+1
        tails.reserve(nums.size());

        for (int num : nums) {
            auto it = std::lower_bound(tails.begin(), tails.end(), num); // subseq be strictly incre order
            if (it == tails.end()) { tails.push_back(num); }
            else { *it = num; } // replace at sep
        } 
        return static_cast<int>(tails.size());
    }
};
