// binary-search on answer

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
            if (canSplice(nums, k, mid)) { hi = mid; } // mid might be true answer
            else { lo = mid + 1; }
        }
        return static_cast<int>(lo);
    }

private:
    bool canSplice(std::vector<int>& nums, int k, long long limit) {
        int parts = 1;
        long long sum = 0;
        for (int num : nums) {
            if (sum + num > limit) {
                parts++;
                sum = num;
            } else {
                sum += num;
            }
        }
        return parts <= k;
    }
};
