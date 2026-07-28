// binary-search

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int n = static_cast<int>(nums.size());        
        int lo = 0; int hi = n; // [lo, hi) ~ [0, n)
        std::vector<int> out(2);

        while (lo < hi) { // lower_bound: 1st idx where nums[i] >= target
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < target) { lo = mid + 1; }
            else { hi = mid; }
        }
        if (lo == n || nums[lo] != target) { return {-1, -1}; } // prune
        out[0] = lo;

        lo = 0; hi = n;
        while (lo < hi) { // upper_bound: 1st idx where nums[i] > target
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] <= target) { lo = mid + 1; }
            else { hi = mid; }
        }
        out[1] = lo - 1;

        return out;
    }
};
