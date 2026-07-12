// generalized recursive K-sum, T: O(n3), S: O(1) with O(k) recursion depth

#include <vector>
#include <algorithm> // std::sort

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
                if (sum < target) {
                    left++;
                } else if (sum > target) {
                    right--;
                } else {
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

            long long min_sum = 0;
            for (int t = 0; t < k; t++) { min_sum += nums[i + t]; }
            if (min_sum > target) { break; }
            
            long long max_sum = nums[i];
            for (int t = 0; t < k - 1; t++) { max_sum += nums[n - 1 - t]; }
            if (max_sum < target) { continue; }

            auto sub_out = kSum(nums, i + 1, k - 1, target - nums[i]);
            for (auto& sub : sub_out) {
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

// pruning checks: min_sum and max_sum
