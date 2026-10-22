// prefix-sum + remainder hash-map, T: O(n), S: O(min(n, k))

#include <vector>
#include <unordered_map>

class Solution {
public:
    bool checkSubarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> remIdx; // remainer: earliest-idx
        remIdx.max_load_factor(0.25f);
        remIdx.reserve(nums.size());
        remIdx[0] = -1;

        long long sum = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            sum += nums[i];
            int rem = sum % k;

            auto it = remIdx.find(rem);
            if (it != remIdx.end()) {
                if (i - it->second >= 2) { return true; }
            } else {
                remIdx[rem] = i; // keep only the earliest idx for remainder
            }
        }
        return false;
    }
};
