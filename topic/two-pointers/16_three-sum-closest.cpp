// sort + two-pointers, T: O(n2), S: O91)

#include <vector>
#include <algorithm> // std::sort
#include <cstdlib> // std::abs

class Solution {
public:
    int threeSumClosest(std::vector<int>& nums, int target) {
        std::sort(nums.begin(), nums.end());
        const int n = static_cast<int>(nums.size());

        int closest = nums[0] + nums[1] + nums[2]; // seed with real triplet
        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) { continue; } // skip num1
            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == target) { return sum; }
                if (std::abs(sum - target) < std::abs(closest - target)) { closest = sum; }

                if (sum < target) { left++; }
                else { right--; }
            }
        }
        return closest;
    }
};
