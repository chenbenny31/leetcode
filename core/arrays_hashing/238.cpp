// prefix and suffix auxiliary, T: O(n), S: O(n)

// two pass carrying scalar, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        std::vector<int> out(n);
        out[0] = 1;

        for (int i = 1; i < n; i++) { out[i] = out[i - 1] * nums[i - 1]; }

        int suf_prod = 1;
        for (int i = n - 1; i >= 0; i--) {
            out[i] *= suf_prod;
            suf_prod *= nums[i];
        }
        return out;
    }
};

// no overflow is guaranteed by problem: otherwise product need long long scratch
// zeros fall out for free: prod is safe for 0, div need cases
