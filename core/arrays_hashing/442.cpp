// hash-set, T: O(n), S: O(n)
// sort and scan, T: O(nlogn), S: O(1)

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

// cycle-sort, T: O(n) amortized, S: O(1)

#include <vector>
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

// no sanitize and bound-check due to question contraints 1<=nums[i]<=n
// arithmetic accm marking: nums[(nums[i]-1)%n] instead of negating, check nums[i] > 2n to identify dup
// no reading a stale va after negation: compute idx from nums[i] with abs() first, then negate it
