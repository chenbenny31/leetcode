// cycle-sort, T: O(n) amortized, S: O(1)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    int firstMissingPositive(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; i++) {
            while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
                int correct = nums[i] - 1;
                std::swap(nums[i], nums[correct]);
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { return i + 1; }
        }
        return n + 1;
    }
};

// sign-marking, T: O(n) worst-case, T: O(1)

#include <cstdlib> // std::abs

class Solution {
public:
    int firstMissingPositive(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        // step 1: sanitize
        for (int i = 0; i < n; i++) {
            if (nums[i] <= 0 || nums[i] > n) { nums[i] = n + 1; }
        }

        // step 2: mark val by negating nums[v-1]
        for (int i = 0; i < n; i++) {
            int v = std::abs(nums[i]);
            if (v >= 1 && v <= n && nums[v - 1] > 0) { nums[v - 1] = -nums[v - 1]; }
        }

        // step 3: first pos slot is the missing val
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) { return i + 1; }
        }
        return n + 1;
    }
};
