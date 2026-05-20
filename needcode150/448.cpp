// flat-array, T: O(n), S: O(n) stack-alloc

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<int> findDisappearedNumbers(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        constexpr int R = 100'001;
        bool seen[R];
        std::memset(seen, 0, sizeof(seen));

        for (int x : nums) { seen[x] = true; }

        std::vector<int> out;
        for (int i = 1; i <= n; i++) {
            if (!seen[i]) { out.push_back(i); }
        }
        return out;
    }
};

// index-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findDisappearedNumbers(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        for (int x : nums) {
            int idx = std::abs(x) - 1;
            if (nums[idx] > 0) { nums[idx] = -nums[idx]; }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) { out.push_back(i + 1); }
        }
        return out;
    }
};

// cycle-sort, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> findDisappearedNumbers(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int i = 0;
        while (i < n) {
            int correct = nums[i] - 1;
            if (nums[i] != nums[correct]) { std::swap(nums[i], nums[correct]); }
            else { ++i; }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { out.push_back(i + 1); }
        }
        return out;
    }
};

// index-marking: `nums[idx] > 0` guard, prevents double-negation
// cycle-sort identical for both missing and duplicate
// flat-array size: `bool seen[100'001]` ~ 100KB, fits L2
