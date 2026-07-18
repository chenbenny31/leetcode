// prefix-sum + remainder hash-map, T: O(n), S: O(min(n, k))

#include <vector>
#include <unordered_map>

class Solution {
public:
    bool checkSubarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> remIdx;
        remIdx.reserve(nums.size());
        remIdx.max_load_factor(0.25f);
        remIdx[0] = -1;

        int sum = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            sum += nums[i];
            int rem = sum % k;

            auto it = remIdx.find(rem);
            if (it != remIdx.end()) {
                if (i - it->second >= 2) { return true; }
            } else {
                remIdx[rem] = i;
            }
        }
        return false;
    }
};

// record only the first occurrence of each remainder: length is at least two
// remIdx[0] = -1: make prefix compute valid
