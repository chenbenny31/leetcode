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

#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        constexpr int BASE = -10'000;
        constexpr int R = 20'001;

        std::vector<int> table(R, -1);

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int val = nums[i];
            int need = target - val;
            if (need >= BASE && need < BASE + R && table[need - BASE] != -1) {
                return {table[need - BASE], i};
            }
            table[val - BASE] = i;
        }
        return {};
    }
};

// direct-indexed flat array: lookup has zero hash compute and pointer chasing, R=1e5: L2-resident, R=1e6-1e7: L3-resident
// unordered_map: buckets point to sep heal-alloc nodes, risk a cold cache line
// reserve + max_load_factor(0.25f): guarantees no rehash during single pass
// -1 as flat-array sentinel: index 0 is legal answer pos, -1 is never valid
// branch behavior: if != seen.end() is false except the find one, predictor settles into "not taken", only one mis-prediction
