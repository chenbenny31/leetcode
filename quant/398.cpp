// precompute + hash-map

#include <vector>
#include <unordered_map>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution

class Solution {
private:
    std::unordered_map<int, std::vector<int>> idxMap; // val: vector<val>
    std::mt19937 rng;

public:
    Solution(vector<int>& nums) : rng(std::random_device{}()) {
        idxMap.reserve(nums.size());
        idxMap.max_load_factor(0.25f);
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            idxMap[nums[i]].push_back(i);
        }
    }

    int pick(int target) {
        const std::vector<int> indices = idxMap.at(target); // at over [], avoid phantom-insert
        std::uniform_int_distribution<int> dist(0, static_cast<int>(indices.size()) - 1);
        return indices[dist(rng)];
    }
};

// reservoir sampling

#include <vector>
#include <random>

class Solution {
private:
    std::vector<int> vals;
    std::mt19937 rng;

public:
    Solution(std::vector<int>& nums) : vals(nums), rng(std::random_device{}()) {}

    int pick(int target) {
        int count = 0;
        int result = -1;

        for (int i = 0; i < static_cast<int>(vals.size()); i++) {
            if (vals[i] == target) {
                count++;
                std::uniform_int_distribution<int> dist(0, count - 1);
                if (dist(rng) == 0) { result = i; } // each val's pick prob is 1/count
            }
        }
        return result;
    }
};
