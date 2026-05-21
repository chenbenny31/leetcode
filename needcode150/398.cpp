// hash-map, T: O(1) pick, T: O(n)

#include <vector>
#include <unordered_map>
#include <cstdlib> // std::rand

class Solution {
    std::unordered_map<int, std::vector<int>> indices_map; // val: [idx,]

public:
    Solution(std::vector<int>& nums) {
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            indices_map[nums[i]].push_back(i);
        }
    }

    int pick(int target) {
        auto& indices = indices_map[target];
        return indices[std::rand() % indices.size()];
    }
};

// reservoir sampling, T: O(n) per pick, S: O(1)

#include <vector>
#include <cstdlib> // std::rand

class Solution {
    std::vector<int> vals;

public:
    Solution(std::vector<int>& nums) : vals(std::move(nums)) {}

    int pick(int target) {
        int res = -1;
        int count = 0;
        for (int i = 0; i < static_cast<int>(vals.size()); i++) {
            if (vals[i] != target) { continue; }
            ++count;
            if (std::rand() % count == 0) { res = i; }
        }
        return res;
    }
};

// hash-map vs reservoir: call freq tradeoff, reservoir is more caceh friendly
// std::rand() % count == 0: prob 1/count
// O(logn) pick: sort (val, idx) pair then binary-search for range
