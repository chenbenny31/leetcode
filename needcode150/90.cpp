// backtracking with duplicate skip, T: O(n2^n), S: O(n) recursion path

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    std::vector<std::vector<int>> subsetsWithDup(std::vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        std::vector<std::vector<int>> out;
        out.reserve(1 << nums.size());
        std::vector<int> curr;
        backtrack(nums, 0, curr, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& nums, int start,
                   std::vector<int>& curr, std::vector<std::vector<int>>& out) {
        out.push_back(curr);

        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            if (i > start && nums[i] == nums[i - 1]) { continue; }
            curr.push_back(nums[i]);
            backtrack(nums, i + 1, curr, out);
            curr.pop_back();
        }
    }
};

// i > start && nums[i] == nums[i-1]: skip duplicate at same recursion level
