// swap + reverse, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::swap, std::reverse

class Solution {
public:
    void nextPermutation(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        int i = n - 2;
        while (i >= 0 && nums[i] >= nums[i + 1]) { i--; } // suffix [i..n) is non-incre

        if (i >= 0) {
            int j = n -1;
            while (nums[j] <= nums[i]) { j--; }
            std::swap(nums[i], nums[j]); // swap i with next max at j
        }

        std::reverse(nums.begin() + i + 1, nums.end()); // restore suffix to non-decre
    }
};
