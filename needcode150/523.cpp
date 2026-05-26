// prefix-sum + hash-map, T: O(n), S: O(k)

#include <vector>
#include <unordered_map>

class Solution {
public:
    bool checkSubarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> first_seen;
        first_seen.reserve(nums.size()); // k could trigger overflow k = 10^9
        first_seen.max_load_factor(0.25f);
        first_seen[0] = -1;

        int prefix = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            prefix += nums[i];
            int rem = prefix % k;
            auto it = first_seen.find(rem);
            if (it != first_seen.end()) {
                if (i - it->second >= 2) { return true; }
            } else {
                first_seen[rem] = i;
            }
        }
        return false;
    }
};

// store first occurrence not last: need the longest possible subarray
// first_seen[0] = -1: sentinal handles subarray starting from idx 0, prefix[i] % k == 0
// i - it->second >= 2: subarray at least two elems
// reserve(k) not reserve(nums.size()): remainders in [0, k-1]
// cache for hash-map with small k, fits in L1, absl::flat_hash_map for large k
