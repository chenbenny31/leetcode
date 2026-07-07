// sort + adjacent-scan, T: O(nlogn), S: O(1)

// hash-set, T: O(n) average, S: O(n), S: O(n)

#include <vector>
#include <unordered_set>

class Solution {
public:
    bool containsDuplicate(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        std::unordered_set<int> seen;
        seen.reserve(n);
        seen.max_load_factor(0.25f);

        for (int i = 0; i < n; i++) {
            auto res = seen.insert(nums[i]);
            if (!res.second) { return true; }
        }
        return false;
    }
};

// insert() not count(): avoid dup hasing, util insert's return std::pair<iterator, bool>, .second=false mean dup
