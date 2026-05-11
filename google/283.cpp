// two-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::swap

class Solution {
public:
    void moveZeros(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int left = 0; // next non-zero pos
        for (int right = 0; right < n; ++right) {
            if (nums[right] != 0) { 
                std::swap(nums[left], nums[right]);
                ++left;
            }
        }
    }
};

// swap not overwrite + fill: one pass and preserve relative order
