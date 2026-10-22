// sign-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        std::vector<int> out;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int idx = std::abs(nums[i]) - 1;
            if (nums[idx] < 0) {
                out.push_back(std::abs(nums[i]));
            } else {
                nums[idx] *= -1;
            }
        }
        return out;
    }
};

// cycle-sort, T: O(n), S: O(1)

#include <utility> // std::swap

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; i++) {
            while (nums[nums[i] - 1] != nums[i]) {
                std::swap(nums[i], nums[nums[i] - 1]);
            }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) {
                out.push_back(nums[i]);
            }
        }
        return out;
    }
};
