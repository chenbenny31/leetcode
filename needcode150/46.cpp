// backtracking with swap, T: O(nn!), S: O(n) recursion path

#include <vector>
#include <algorithm> // std::swap

class Solution {
public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<std::vector<int>> out;
        backtrack(nums, 0, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& nums, int start,
                   std::vector<std::vector<int>>& out) {
        if (start == static_cast<int>(nums.size())) { out.push_back(nums); return; }

        for (int i = start; i < static_cast<int>(nums.size()); i++) {
            std::swap(nums[start], nums[i]);
            backtrack(nums, start + 1, out);
            std::swap(nums[start], nums[i]);
        }
    }
};

// swap-based: no extra space, in-place nums modification
// out.reserve n! permutations:
int fact = 1;
for (int i = 1 ; i <= static_cast<int>(nums.size()); i++) { fact *= i; }
out.reserve(fact);
