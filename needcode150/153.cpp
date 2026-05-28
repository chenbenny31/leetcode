// binary-search, T: O(log(n)), S: O(1)

#include <vector>

class Solution {
public:
    int findMin(std::vector<int>& nums) {
        int lo = 0;
        int hi = static_cast<int>(nums.size()) - 1;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            
            if (nums[mid] > nums[hi]) { lo = mid + 1; } // min in right half
            else { hi = mid; } // min in left half or mid
        }
        return nums[lo];
    }
};

// compare nums[mid] vs nums[hi] not nums[lo]: nums[mid] > nums[hi] means rotation point in (mid, hi]
// lo < hi, hi = mid: half-open invariant, hi = mid never excludes mid, could be min
