// in-place, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::reverse, std::swap

class Solution {
public:
    void nextPermutation(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());

        // find rightmost i where nums[i] < nums[i + 1]
        int i = n - 2;
        while (i >= 0 && nums[i] >= nums[i + 1]) { i--; }

        // find rightmost j where nums[j] > nums[i]
        if (i >= 0) {
            int j = n - 1;
            while (nums[j] <= nums[i]) { j--; }
            std::swap(nums[i], nums[j]);
        }

        // reverse suffix [i + 1, n - 1]
        std::reverse(nums.begin() + i + 1, nums.end());
    }
};

// step 1 finds incre point nums[i] < nums[i+1]; step 2 finds smalles elem to the right > nums[i]; step 3 reverse suffix to incre
// std::reverse: O(n) in-place, compiles to vectorized swap at -O2
// i < 0 guard non-incre
