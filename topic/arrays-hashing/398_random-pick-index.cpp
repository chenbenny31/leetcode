// precompute hash-map, T: O(1) pick, S: O(n)

#include <vector>
#include <unordered_map>
#include <cstdlib> // std::srand, std::rand
#include <ctime> // std::time

class Solution {
private:
    std::unordered_map<int, std::vector<int>> map; // val: [idx,]

public:
    Solution(std::vector<int>& nums) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        map.max_load_factor(0.25f);
        map.reserve(nums.size());
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            map[nums[i]].push_back(i);
        }
    }

    int pick(int target) {
        const std::vector<int>& indices = map.at(target);
        return indices[std::rand() % indices.size()];
    }
};

// reservoir-sampling, T: O(n) pick, S: O(1)
// follow-up: no space allocation || stream input

class Solution {
private:
    std::vector<int> nums;

public:
    Solution(std::vector<int>& nums) : nums(nums) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    int pick(int target) {
        int count = 0;
        int result = -1;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] == target) {
                count++;
                if (std::rand() % count == 0) { result = i; } // fire with prob 1/count
            }
        }
        return result;
    }
};
