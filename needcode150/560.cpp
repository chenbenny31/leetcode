// prefix-sum + hash-map, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>

class Solution {
public:
    int subarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> freq;
        freq.reserve(nums.size());
        freq.max_load_factor(0.25f);
        freq[0] = 1; // empty prefix

        int sum = 0;
        int cnt = 0;
        for (int x : nums) {
            sum += x;
            auto it = freq.find(sum - k);
            if (it != freq.end()) { cnt += it->second; }
            freq[sum]++;
        }
        return cnt;
    }
};

// freq[0] = 1 sentinel: handles subarrays starting from index 0
// freq[sum]++ after lookup: prevent curr pair itself
// store freq over existence: multiple subarrays share the same prefix sum
// prefix sum could be neg: flat array not viable
// freq[sum]++ not freq.insert: insert require check existence first
