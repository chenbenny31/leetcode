// LC 153 Find Minimum in Rotated Sorted Array (Medium). Pattern: binary search vs right
// end.
// nums is ascending with distinct values, then rotated; return the minimum in O(log n).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// The min always lies in [lo, hi]. Compare with nums[hi]: if nums[mid] > nums[hi] the
// drop is right of mid, else mid is in the lower run. T: O(log n), S: O(1).
int findMin(const vector<int>& nums) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findMin(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rotated", {5, 6, 1, 2, 3, 4}, 1);
    test("not-rotated", {1, 2, 3}, 1);
    test("two-elements", {2, 1}, 1);
    test("single", {7}, 7);
}

// follow-up: why nums[hi], not nums[lo] -> an unrotated array has nums[mid] > nums[lo]
//   too, yet the min is left; nums[hi] separates the two runs in every case
// follow-up: duplicates (LC 154) -> nums[mid] == nums[hi]: hi-- (safe, a copy remains),
//   O(n) worst case
