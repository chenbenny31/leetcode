// backtracking with visited array + duplicate skip, T: O(nn!), S: O(n) recursion path

#include <vector>
#include <algorithm> // std::sort
#include <cstdint> // uint8_t

class Solution {
public:
    std::vector<std::vector<int>> permuteUnique(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        std::vector<std::vector<int>> out;
        std::vector<int> curr;
        curr.reserve(nums.size());
        std::vector<uint8_t> used(nums.size(), 0);
        backtrack(nums, used, curr, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& nums, std::vector<uint8_t>& used,
                   std::vector<int>& curr, std::vector<std::vector<int>>& out) {
        if (curr.size() == nums.size()) { out.push_back(curr); return; }

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (used[i]) { continue; }
            if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) { continue; }
            used[i] = 1;
            curr.push_back(nums[i]);
            backtrack(nums, used, curr, out);
            curr.pop_back();
            used[i] = 0;
        }
    }
};

// i > 0 && nums[i] == nums[i-1] && !used[i-1]: core dedeuplication
// uint8_t not vector<bool>: vector<bool> bit-packs, proxy objects, no real references, while uint8_t gives real bytes per element
// sort mandatory: nums[i] == nums[i-1] requires identitcal element adjacent
// why swap approach break: swap changes relative order of elements, duplicate detection via nums[i] == nums[i-1] becomes unreliable after swapping
// out over-reserve:
int fact = 1;
for (int i = 1; i <= static_cast<int>(nums.size()); i++) { fact *= 1; }
out.reserve(fact);
