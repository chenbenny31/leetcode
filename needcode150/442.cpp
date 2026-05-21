// flat-array, T: O(n), S: O(n) stack-alloc

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        constexpr int R = 100001;
        bool seen[R];
        std::memset(seen, 0, sizeof(seen));

        std::vector<int> out;
        for (int x : nums) {
            if (seen[x]) { out.push_back(x); }
            else { seen[x] = true; }
        }
        return out;
    }
};

// index-marking, values in [1, n] map to indices [0, n-1], negate nums[|x|-1] as visited flag, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        std::vector<int> out;
        for (int x : nums) {
            int idx = std::abs(x) - 1; // map to 0-indexed
            if (nums[idx] < 0) { out.push_back(std::abs(x)); }
            else { nums[idx] = -nums[idx]; }
        }
        return out;
    }
};

// cycle-sort, nums[i]=i+1, after sorting indices nums[i]!=i+1 hold duplicates, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::swap

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int i = 0;
        while (i < n) {
            int correct = nums[i] - 1;
            if (nums[i] != nums[correct]) { std::swap(nums[i], nums[correct]); }
            else { i++; }
        }

        std::vector<int> out;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { out.push_back(nums[i]); }
        }
        return out;
    }
};

// index-marking: sign bit as flag, only all vals pos and fits in signed int
// cycle-sort termination: each swap reduce at least on elem pos, out-of-range will skip
// restore after index-marking: for (int& x : nums) { x = std::abs(x); }
