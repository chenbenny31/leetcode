// greedy

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int jump(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        int jumps = 0;
        int curEnd = 0;
        int maxReach = 0;

        for (int i = 0; i < n - 1; i++) {
            maxReach = std::max(maxReach, i + nums[i]);
            if (i == curEnd) {
                jumps++;
                curEnd = maxReach;
            }
        }
        return jumps;
    }
};
