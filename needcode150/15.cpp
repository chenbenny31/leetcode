// sort + two-pointers, T: O(n2), S: O(1)

#include <vector>
#include <algorithm>

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        int n = static_cast<int>(nums.size());
        std::vector<std::vector<int>> out;

        for (int i = 0; i < n - 2; i++) {
            if (nums[i] > 0) { break; } // prune: first num must be neg since sorted
            if (i > 0 && nums[i] == nums[i - 1]) { continue; } // skip dup i

            int left = i + 1;
            int right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    out.push_back({nums[i], nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) { ++left; } // skip dup left
                    while (left < right && nums[right] == nums[right - 1]) { --right; } // skip dup left
                    ++left;
                    --right;
                } else if (sum < 0) {
                    ++left;
                } else {
                    --right;
                }
            }
        }
        return out;
    }
};


// dup skip: outer i; inner left/right
// early exit: nums[i] > 0
