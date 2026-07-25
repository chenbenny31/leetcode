// hash-map, T: O(n), S: O(n)

#include <unordered_map>
#include <vector>

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, int> seen; // nums[i]: i 

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            int need = target - nums[i];
            auto it = seen.find(need);
            if (it != seen.end()) { return {it->second, i}; }
            seen[nums[i]] = i;
        }
        return {-1, -1};
    }
};

// flat-array, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = static_cast<int>(nums.size());
        int R = 20001;
        int BASE = -10001;
        vector<int> seen(R, -1);

        for (int i = 0; i < n; i++) {
            int need = target - nums[i];
            int idx = seen[need - BASE];
            if (idx != -1) { return {idx, i}; }
            seen[nums[i] - BASE] = i;
        }
        return {-1, -1};
    }
};
