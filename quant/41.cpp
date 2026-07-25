// cycle-sort

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = static_cast<int>(nums.size());         
        for (int i = 0; i < n; i++) {
            while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) { // until place correct val in current pos or exhaust
                std::swap(nums[i], nums[nums[i] - 1]);
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { return i + 1; }
        }
        return n + 1;
    }
};

// sign-marking

#include <cstdlib> // std::abs

class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        for (int i = 0; i < n; i++) {
            if (nums[i] <= 0) { nums[i] = n + 1; }
        }
        for (int i = 0; i < n; i++) {
            int v = std::abs(nums[i]); // gated v for range check
            if (v >= 1 && v <= n) { nums[v - 1] = -std::abs(nums[v - 1]); }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) { return i + 1; }
        }
        return n + 1;
    }
};
