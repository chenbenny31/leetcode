// track min and max, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::min

class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        int curMax = nums[0];
        int curMin = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            int x = nums[i];
            int tmpMax = std::max({x, curMax * x, curMin * x});
            int tmpMin = std::min({x, curMax * x, curMin * x});
            curMax = tmpMax;
            curMin = tmpMin;
            best = std::max(best, curMax);
        }
        return best;
    }
};

// track both min and max: negative * negative = positive
// tmp max and min before update cur max and min: preserve prev cur max/min vals
// three-way std::max/std::min: compiles to two comparison
