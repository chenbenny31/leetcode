// sort + two-pointers

#include <vector>
#include <algorithm>

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        std::vector<std::vector<int>> out;
        if (n < 3) { return out; }
        std::sort(nums.begin(), nums.end());

        for (int i = 0; i < n; i++) {
            if (i > 0 && nums[i] == nums[i - 1]) { continue; }
            if (nums[i] > 0) { break; }

            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum < 0) { left++; }
                else if (sum > 0) { right--; }
                else {
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
