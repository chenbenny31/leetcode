// sort + adjacent-scan, T: O(nlogn), S: O(1)

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    bool containsDuplicate(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] == nums[i - 1]) { return true; }
        }
        return false;
    }
};


// hash-set, T: O(n) average, S: O(n), S: O(n)

#include <unordered_set>

class Solution {
public:
    bool containsDuplicate(std::vector<int>& nums) {
        std::unordered_set<int> seen;
        seen.reserve(nums.size());

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (!seen.insert(nums[i]).second) { return true; } // insert returns {iterator, bool}
        }
        return false;
    }
};
