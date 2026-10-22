// hash-map, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        std::unordered_map<int, int> seen; // val: idx
        seen.reserve(nums.size());
        seen.max_load_factor(0.25f);

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int need = target - nums[i];
            auto it = seen.find(need);
            if (it != seen.end()) { return {it->second, i}; }
            seen[nums[i]] = i;
        }
        return {};
    }
};

// direct indexed flat array, T: O(n), S: O(R) R = bounded range

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        constexpr int BASE = -10'000;
        constexpr int R = 20'001;

        std::vector<int> table(R, -1);

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int need = target - nums[i];
            if (need >= BASE && need < BASE + R && table[need - BASE] != -1) {
                return {table[need - BASE], i};
            }
            table[nums[i] - BASE] = i;
        }
        return {};
    }
};
