// binary-search, T: O(log(n)), S: O(1)

#include <vector>

class Solution {
public:
    int findPeakElement(std::vector<int>& nums) {
        int lo = 0;
        int hi = static_cast<int>(nums.size()) - 1;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < nums[mid + 1]) { lo = mid + 1; }
            else { hi = mid; }
        }
        return lo;
    }
};

// binary on unsorted array: nums[-1] = nums[n] = -infi, all elems uniq, any nums[mid] < nums[mid+1] guarantees peak to the right
// lo < hi: half-open invariant converges to single elem, hi = mid preserve mid prob to be peak
