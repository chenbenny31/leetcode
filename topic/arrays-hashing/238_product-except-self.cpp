// two pass carrying scalar, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        std::vector<int> out(n);

        out[0] = 1;
        for (int i = 1; i < n; i++) {
            out[i] = out[i - 1] * nums[i - 1];
        }

        int suffix = 1;
        for (int i = n - 1; i >= 0; i--) {
            out[i] *= suffix;
            suffix *= nums[i];
        }
        return out;
    }
};
