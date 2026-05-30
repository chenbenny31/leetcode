// binary-search on answer, T: O(nlog(sum-max)), S: O(1)

#include <vector>
#include <numeric> // std::accumulate
#include <algorithm> // std::max_element

class Solution {
public:
    int splitArray(std::vector<int>& nums, int k) {
        long long lo = *std::max_element(nums.begin(), nums.end());
        long long hi = std::accumulate(nums.begin(), nums.end(), 0LL);

        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (canSplit(nums, k, mid)) { hi = mid; }
            else { lo = mid + 1; }
        }
        return static_cast<int>(lo);
    }

private:
    bool canSplit(std::vector<int>& nums, int k, long long limit) {
        int parts = 1;
        long long sum = 0;
        for (int x : nums) {
            if (sum + x > limit) {
                ++parts;
                sum = x;
            } else {
                sum += x;
            }
        }
        return parts <= k;
    }
};

// long long for lo, hi: sum(nums) could overflow int
// std::accumulate with 0LL force long long casting
// greedy feasibility: greedily fill each subarray
// vs dp solution: dp[i][j] = min largest sum splitting first i elems into j parts, T: O(kn2), S: O(kn)
