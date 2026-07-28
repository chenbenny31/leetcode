// binary-search

class Solution { // lower_bound
public:
    int findMin(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int lo = 0;
        int hi = n - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] > nums[hi]) { lo = mid + 1; }
            else { hi = mid; }
        }
        return nums[lo];
    }
};

class Solution { // upper_bound
public:
    int findMin(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int lo = 0;
        int hi = n - 1;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] >= nums[0]) { lo = mid + 1; }
            else { hi = mid; }
        }
        return (nums[lo] < nums[0]) ? nums[lo] : nums[0];
    }
};
