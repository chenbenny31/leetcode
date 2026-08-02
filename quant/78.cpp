// backtracking

#include <vector>

class Solution {
private:
    void backtrack(std::vector<int>& nums, int start, std::vector<int> path,
                   std::vector<std::vector<int>>& out) {
        out.push_back(path);

        for (int i = start; i < static_cast<int>(nums.size()); i++) {
            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, out);
            path.pop_back();
        }
    }
                    
public:
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        std::vector<std::vector<int>> out;
        out.reserve(1u << nums.size());

        std::vector<int> path;
        path.reserve(nums.size());

        backtrack(nums, 0, path, out);
        return out;
    }
};

// bitmask

#include <utility>

class Solution {
public:
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        const int total = 1 << n;

        std::vector<std::vector<int>> out;
        out.reserve(total);

        for (int mask = 0; mask < total; mask++) {
            std::vector<int> subset;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    subset.push_back(nums[i]);
                }
            }
            out.push_back(std::move(subset));
        }
        return out;
    }
};
