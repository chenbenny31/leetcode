// track min and mac, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::min

class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        int curMax = nums[0];
        int curMin = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            int v = nums[i];
            int tmpMax = std::max({v, curMax * v, curMin * v});
            int tmpMin = std::min({v, curMax * v, curMin * v});
            curMax = tmpMax;
            curMin = tmpMin;
            best = std::max(best, curMax);
        }
        return best;
    }
};

// prefix/suffix product, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxProduct(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int best = nums[0];
        int prefix = 1, suffix = 1;

        for (int i = 0; i < n; i++) {
            prefix = prefix * nums[i];
            suffix = suffix * nums[n-1-i];
            best = std::max({best, prefix, suffix});
            if (prefix == 0) { prefix = 1; }
            if (suffix == 0) { suffix = 1; }
        }
        return best;
    }
};

// track both min and max: negative * negative can produce positive
// prefix/suffix insight: only 0 breaks max/min accumulate
// tmpMax/tmpMin before assignment: need curMax/curMin preserve values first
