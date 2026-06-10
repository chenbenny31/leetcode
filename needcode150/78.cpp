// backtracking, T: O(n2^n), S: O(n) recursion path

#include <vector>

class Solution {
public:
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        std::vector<std::vector<int>> out;
        std::vector<int> curr;
        backtrack(nums, 0, curr, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& nums, int start, std::vector<int>& curr,
                   std::vector<std::vector<int>>& out) {
        out.push_back(curr);

        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            curr.push_back(nums[i]);
            backtrack(nums, i + 1, curr, out);
            curr.pop_back();
        }
    }
};

// bitmask, T: O(n2^n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int total = 1 << n;
        std::vector<std::vector<int>> out;
        out.reserve(total);

        for (int mask = 0; mask < total; ++mask) {
            std::vector<int> curr;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) { curr.push_back(nums[i]); }
            }
            out.push_back(std::move(curr));
        }
        return out;
    }
};

// out.push_back(curr) at every node: subtsets include all intermediate states, from empty set at root to full set at leaf
// out.reserve(1 << nums.size()): exact output size known for 2^n subsets
// bitmask - branch-free: mask & (1 << i), compiler emits SIMD for inner loop at -O2
// std::move(curr) in bitmask: curr is a temporary built per mask, O(1) transfer
