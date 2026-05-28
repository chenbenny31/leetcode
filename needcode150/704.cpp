// binary-search, T: O(log(n)), S: O(1)

#include <vector>

class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int lo = 0;
        int hi = static_cast<int>(nums.size()) - 1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;

            if (nums[mid] == target) { return mid; }
            else if (nums[mid] < target) { lo = mid + 1; }
            else { hi = mid - 1; }
        }
        return -1;
    }
};

// lo <= hi: closed interval [lo, hi] exact match, return immediately on hit
// branch prediction: three-way is data dependent, hard to predict, branchless two-way
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (nums[mid] < target) { lo = mid + 1; }
    else { hi = mid; }
}
return nums[lo] == target ? lo : -1;
// truely branchless version
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    bool cond = nums[mid] < target;
    lo = cond ? mid + 1 : lo;
    hi = cond ? hi : mid;
}
return nums[lo] == target ? lo : -1;
// linear scan crossover: std::lower_bound in libstdc++ switches to linear scan
