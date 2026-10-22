// generalized recursive K-sum, T: O(n3), S: O(1) aux + O(k) recursion

#include <vector>
#include <algorithm> // std::sort
#include <utility> // std::move

class Solution {
private:
    std::vector<std::vector<int>> kSum(std::vector<int>& nums, int start, int k, long long target) {
        std::vector<std::vector<int>> out;
        const int n = static_cast<int>(nums.size());

        if (k == 2) {
            int left = start;
            int right = n - 1;
            while (left < right) {
                long long sum = static_cast<long long>(nums[left]) + nums[right];
                if (sum < target) { left++; }
                else if (sum > target) { right--; }
                else {
                    out.push_back({nums[left], nums[right]});
                    left++; right--;
                    while (left < right && nums[left] == nums[left - 1]) { left++; }
                    while (left < right && nums[right] == nums[right + 1]) { right--; }
                }
            }
            return out;
        }

        for (int i = start; i <= n - k; i++) {
            if (i > start && nums[i] == nums[i - 1]) { continue; }

            long long minSum = 0;
            for (int t = 0; t < k; t++) { minSum += nums[i + t]; }
            if (minSum > target) { break; }

            long long maxSum = 0;
            for (int t = 0; t < k; t++) { maxSum += nums[n - 1 - t]; }
            if (maxSum < target) { continue; }

            auto subOut = kSum(nums, i + 1, k - 1, target - nums[i]);
            for (auto& sub : subOut) {
                sub.insert(sub.begin(), nums[i]);
                out.push_back(std::move(sub));
            }
        }
        return out;
    }

public:
    std::vector<std::vector<int>> fourSum(std::vector<int>& nums, int target) {
        std::sort(nums.begin(), nums.end());
        return kSum(nums, 0, 4, static_cast<long long>(target));
    }
};
