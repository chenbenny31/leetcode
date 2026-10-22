// cycle-sort with boundary-adjust for n T: O(n), S: O(1)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; i++) {
            while (nums[i] < n && nums[i] != i) {
                std::swap(nums[i], nums[nums[i]]);
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] != i) { return i; }
        }
        return n;
    }
};

// XOR trick, T: O(n), S: O(1)

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        int res = n; // in [0..n] with no loop-idx counterpart
        for (int i = 0; i < n; i++) {
            res ^= i;
            res ^= nums[i];
        }
        return res;
    }
};
