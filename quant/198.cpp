// dp

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int rob(vector<int>& nums) {
        const int n = static_cast<int>(nums.size());       
        if (n == 1) { return nums[0]; }
        int prev2 = 0; // best up to i-2
        int prev1 = 0; // best up to i-1

        for (int i = 0; i < n; i++) {
            int curr = std::max(prev1, prev2 + nums[i]);
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
