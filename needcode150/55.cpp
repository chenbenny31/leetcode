// greedy, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    bool canJump(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int furthest = 0;

        for (int i = 0; i < n; i++) {
            if (i > furthest) { return false; }
            furthest = std::max(furthest, i + nums[i]);
        }
        return true;
    }
};

// i > furthest: early exit
// greedy correctness: furtherest reachable index is monotonically tracked in max, classical interval-covering greedy
