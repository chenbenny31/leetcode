// Floyd's Cycle Detection. T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int slow = 0;
        int fast = 0;

        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        slow = 0;
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }
        return slow;
    }
};

// negative marking, T: O(n), S: O(1)

#include <cstdlib> // std::abs

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]);
            if (nums[idx] < 0) { return idx; }
            nums[idx] = -std::abs(nums[idx]);
        }
        return -1;
    }
};
