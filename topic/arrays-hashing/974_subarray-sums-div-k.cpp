// prefix remainder counting, T: O(n), S: O(k)

#include <vector>

class Solution {
public:
    int subarraysDivByK(std::vector<int>& nums, int k) {
        std::vector<int> counts(k, 0);
        counts[0] = 1; // the empty prefix, remainer 0

        int sum = 0;
        int result = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            sum += nums[i];
            int r = ((sum % k) + k) % k; // negative % produces negative
            result += counts[r];
            counts[r]++;
        }
        return result;
    }
};
