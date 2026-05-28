// binary-search, T: O(logn), S: O(1)

#include <vector>

class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int lo = 0;
        int hi = static_cast<int>(nums.size()) - 1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;

            if (nums[mid] == target) { return mid; }

            if (nums[lo] <= nums[mid]) { // left half is sorted
                if (target >= nums[lo] && target < nums[mid]) { hi = mid - 1; }
                else { lo = mid + 1; }
            } else { // right half is sorted
                if (target > nums[mid] && target <= nums[hi]) { lo = mid + 1; }
                else { hi = mid - 1; }
            }
        }
        return -1;
    }
};

// key invariant: at least half is sorted
// nums[lo] <= nums[mid]: handles two elem case
// lo <= hi: closed interval for exact match
