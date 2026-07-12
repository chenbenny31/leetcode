// sort + two-pointers, T: O(n2), S: O(1)

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        std::vector<std::vector<int>> out;
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n - 2; i++) {
            if (i > 0 && nums[i] == nums[i -1]) { continue; }
            if (nums[i] > 0) { break; }

            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum < 0) {
                    left++;
                } else if (sum > 0) {
                    right--;
                } else {
                    out.push_back({nums[i], nums[left], nums[right]});
                    left++; right--;
                    while (left < right && nums[left] == nums[left - 1]) { left++; }
                    while (left < right && nums[right] == nums[right + 1]) { right--; }
                }
            }
        }
        return out;
    }
};

// typical bug: silent infinite loop, forget to advance both left/right if match sum
// three separate dup-keip point: nums[i] == nums[i-1], nums[left] == nums[left-1], nums[right] == nums[right+1]
