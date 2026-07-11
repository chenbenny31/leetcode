// hash-set, T: O(n), S: O(n)

// sign-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findDisappearedNumbers(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]) - 1;
            if (nums[idx] > 0) {
                nums[idx] *= -1;
            }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {
                out.push_back(i + 1);
            }
        }
        return out;
    }
};

// cycle-sort, T: O(n) amortized, S: O(1)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    std::vector<int> findDisappearedNumbers(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n; i++) {
            while (nums[nums[i] - 1] != nums[i])  {
                std::swap(nums[i], nums[nums[i] - 1]);
            }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) {
                out.push_back(i + 1);
            }
        }
        return out;
    }
};

// follow-up: add n/modular-arithmetic marking: nums[(nums[i]-1) % n] += n, scan for nums[i] <= n as missing ones
