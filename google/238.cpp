// prefix + suffix pass, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> out(n);

        out[0] = 1;
        for (int i = 1; i < n; ++i) { out[i] = out[i-1] * nums[i-1]; }

        int right = 1;
        for (int i = n-1; i >= 0; --i) {
            out[i] *= right;
            right *= nums[i];
        }
        return out;
    }
};

// single right accumulator over suffix array: save O(n) space cost
