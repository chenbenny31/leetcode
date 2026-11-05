// ByteDance (hack2hire) / LC 33: nums is an ascending array of distinct values rotated
// at an unknown pivot; return the index of target, or -1. O(log n).
// Follow-up LC 81: nums may contain duplicates; return whether target exists.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Binary search; one half of [lo, hi] is always sorted. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[lo] <= nums[mid]) {
            // left half sorted: keep it only if target lies inside its range
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            // right half sorted: keep it only if target lies inside its range
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}

// Follow-up: if lo, mid, hi are equal neither half is known sorted: shrink both ends.
// T: O(n) worst (all equal), O(log n) typical, S: O(1).
bool searchDup(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return true; }
        if (nums[lo] == nums[mid] && nums[mid] == nums[hi]) {
            lo++;
            hi--;
        } else if (nums[lo] <= nums[mid]) {
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return false;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

void testDup(const string& label, const vector<int>& nums, int target, bool want) {
    cout << label << (searchDup(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("found", {4, 5, 6, 7, 0, 1, 2}, 0, 4);
    test("missing", {4, 5, 6, 7, 0, 1, 2}, 3, -1);
    test("single", {1}, 1, 0);
    test("not-rotated", {1, 3, 5}, 5, 2);
    testDup("dup-found", {2, 5, 6, 0, 0, 1, 2}, 0, true);
    testDup("dup-missing", {2, 5, 6, 0, 0, 1, 2}, 3, false);
    testDup("ends-equal", {1, 0, 1, 1, 1}, 0, true);
}

// follow-up: duplicates (LC 81) -> see searchDup() above; lo == mid == hi -> lo++, hi--,
//   O(n) worst
// follow-up: find min / rotation count (LC 153) -> min index = rotation count;
//   dups (LC 154): nums[mid] == nums[hi] -> hi--, O(n) worst
//   while (lo < hi) {
//       int mid = lo + (hi - lo) / 2;
//       if (nums[mid] > nums[hi]) { lo = mid + 1; } else { hi = mid; }
//   }
// follow-up: two-pass -> find pivot (min index) first, then plain binary search on the
//   sorted half that can hold target, O(log n)
