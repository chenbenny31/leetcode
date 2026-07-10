// precompute hash-map, T: O91) pick, S: O(n)

#include <vector>
#include <unordered_map>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution

class Solution {
private:
    std::unordered_map<int, std::vector<int>> idx_map;
    std::mt19937 rng;

public:
    Solution(std::vector<int>& nums) : rng(std::random_device{}()) {
        idx_map.reserve(nums.size());
        idx_map.max_load_factor(0.25f);
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            idx_map[nums[i]].push_back(i);
        }
    }

    int pick(int target) {
        const std::vector<int>& indices = idx_map.at(target);
        std::uniform_int_distribution<int> dist(0, static_cast<int>(indices.size()) - 1);
        return indices[dist(rng)];
    }
};

// reservoir-sampling, T: O(n) pick, S: O(1)

#include <vector>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution

class Solution {
private:
    std::vector<int> nums;
    std::mt19937 rng;

public:
    Solution(std::vector<int>& nums) : nums(nums), rng(std::random_device{}()) {}

    int pick(int target) {
        int count = 0;
        int res = -1;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] == target) {
                count++;
                std::uniform_int_distribution<int> dist(0, count - 1);
                if (dist(rng) == 0) { res = i; }
            }
        }
        return res;
    }
};

// tradeoff: pick is called many times againt the same target prefer 1; memory is constraint or one-pass stream prefer 2
// .at() over operator[]: if target absent, idx_map[targe] will insert an empty vec
// reservoir-sampling correctness: dist(0, count-1) has exactly count possible outcomes, dist(rng)==0 fires with prob at 1/count
