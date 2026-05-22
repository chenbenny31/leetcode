// T: O(n3), S: O(1)

#include <vector>
#include <algorithm> // std:sort

class Solution {
public:
    std::vector<std::vector<int>> fourSum(std::vector<int>& nums, int target) {
        std::sort(nums.begin(), nums.end());
        int n = static_cast<int>(nums.size());
        std::vector<std::vector<int>> out;

        for (int i = 0; i < n - 3; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) { continue; } // skip dup i

            for (int j = i + 1; j < n - 2; j++) {
                if (j > i + 1 && nums[j] == nums[j - 1]) { continue; } // skip dup j

                int left = j + 1;
                int right = n - 1;
                while (left < right) {
                    long long sum = static_cast<long long>(nums[i]) + nums[j] + nums[left] + nums[right];
                    if (sum == target) {
                        out.push_back({nums[i], nums[j], nums[left], nums[right]});
                        while (left < right && nums[left] == nums[left + 1]) { ++left; }
                        while (left < right && nums[right] == nums[right - 1]) { --right; }
                        ++left;
                        --right;
                    } else if (sum < target) {
                        ++left;
                    } else {
                        --right;
                    }
                }
            }
        }
        return out;
    }
};


// long long sum mandatory: 4 * 10^9 overflow int
// kSum generalization: recursive reduction
