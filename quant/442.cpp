// cycle-sort

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        for (int i = 0; i < n; i++) {
            while (nums[nums[i] - 1] != nums[i]) {
                std::swap(nums[nums[i] - 1], nums[i]);
            }
        }
        std::vector<int> out;
        out.reserve(n);
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { out.push_back(nums[i]); }
        }
        return out;
    }
};

// sign-marking

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        std::vector<int> out;
        out.reserve(n);
        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]) - 1;
            if (nums[idx] < 0) { out.push_back(std::abs(nums[i])); }
            else { nums[idx] *= -1; }
        }
        return out;
    }
};
