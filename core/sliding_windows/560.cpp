// prefix-sum + count hash-map, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>

class Solution {
public:
    int subarraySum(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> freq; // prefix-sum: count
        freq.reserve(nums.size());
        freq[0] = 1;

        int sum = 0;
        int count = 0;
        for (int num : nums) {
            sum += num;
            auto it = freq.find(sum - k);
            if (it != freq.end()) { count += it->second; }
            freq[sum]++;
        }
        return count;
    }
};

// freq[0] = 1: empty prefix sentinel, choose count as 1
