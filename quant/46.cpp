// backtrack + swap

#include <vector>
#include <algorithm> // std::swap

class Solution {
private:
    void backtrack(std::vector<int>& nums, int idx, std::vector<std::vector<int>>& out) {
        const int n = static_cast<int>(nums.size());
        if (idx == n) {
            out.push_back(nums);
            return;
        }
        for (int i = idx; i < n; i++) {
            std::swap(nums[idx], nums[i]);
            backtrack(nums, idx + 1, out);
            std::swap(nums[idx], nums[i]);
        }
    }

public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<std::vector<int>> out;
        backtrack(nums, 0, out);
        return out;
    }
};
